#include "GameState.h"
#include <algorithm>
#include <random>
#include <map>
#include <cmath>
//#define M_PI acos(-1)

const float GameState::INITIAL_FALL_SPEED = 0.0f;
const float GameState::MIN_FALL_SPEED = 0.0f;
const float GameState::MAX_FALL_SPEED = 2.8f;

GameState::GameState() :
    fallSpeed(INITIAL_FALL_SPEED),
    lastFallTime(0.0f),
    score(0),
    gameOver(false),
    facedDirection(Front),
    rotating(false),
    currentAngle(0.0f),
    targetAngle(0.0f),
    rotationSpeed(3.0f)
{
    createNewBlock(); //game starts with an active block
}

void GameState::createNewBlock() {

    if (gameOver)
    {
        return;
    }

    activeBlock.clear();
    //fallSpeed = INITIAL_FALL_SPEED;

    for (int x = 3; x < 6; x++) {
        for (int y = 12; y < 15; y++) {
            for (int z = 3; z < 6; z++) {
                Block block = {
                    static_cast<float>(x),
                    static_cast<float>(y),
                    static_cast<float>(z),
                };
                activeBlock.push_back(block);
            }
        }
    }

    if (checkCollision()) {
        gameOver = true;
        activeBlock.clear();
    }
}

void GameState::updateBlockPosition(float currentTime) {
    if (currentTime - lastFallTime >= 1.0f / fallSpeed) {
        // Move block down
        for (auto& block : activeBlock) {
            block.y = round(block.y - 1.0f);
        }

        if (checkCollision()) {
            // Move block back up
            for (auto& block : activeBlock) {
                block.y = round(block.y + 1.0f);
            }
            // Add to placed blocks
            blocks.insert(blocks.end(), activeBlock.begin(), activeBlock.end());
            checkLineCompletion();
            createNewBlock();
            if (checkCollision()) {
                gameOver = true;
            }
        }
        lastFallTime = currentTime;
    }
}


bool GameState::checkBoundariesCollision() {
    for (const auto& block : activeBlock) {
        if (block.y < 0) return true;
        
        if (block.x < 0 || block.x >= BOARD_SIZE ||
            block.z < 0 || block.z >= BOARD_SIZE) {
            return true;
        }
    }
    return false;
}

bool GameState::checkCollision() {
    for (const auto& block : activeBlock) {
        if (block.y < 0) return true;

        for (const auto& placedBlock : blocks) {
            if (block.x == placedBlock.x &&
                block.y == placedBlock.y &&
                block.z == placedBlock.z) {
                return true;
            }
        }
    }
    return false;
}

void GameState::moveBlock(int direction) {

    if (rotating)
        return;

    float dx = 0.0f, dz = 0.0f;

    // Apply view rotation to movement
    float radian = targetAngle * M_PI / 180.0f;
    dx = direction * cos(radian);
    dz = -direction * sin(radian);

    for (auto& block : activeBlock) {
        block.x = round(block.x + dx);
        block.z = round(block.z + dz);
    }

    if (checkCollision() || checkBoundariesCollision()) {
        for (auto& block : activeBlock) {
            block.x = round(block.x - dx);
            block.z = round(block.z - dz);
        }
    }
}

void GameState::updateFallSpeed(float delta) {
    
    float newFallSpeed = fallSpeed + delta;

    if (newFallSpeed > MAX_FALL_SPEED)
    {
        fallSpeed = MAX_FALL_SPEED;
        return;
    }
    if (newFallSpeed < MIN_FALL_SPEED)
    {
        fallSpeed = MIN_FALL_SPEED;
        return;
    }
    fallSpeed = newFallSpeed;
}

void GameState::rotateView(int direction) {

    facedDirection = Direction((facedDirection + 4 + direction) % 4); // -1 % 4 returns -1 in cpp so add a 4
    targetAngle += direction * 90.0f;
    rotating = true; //enter rotating state

    if (targetAngle > 360 && currentAngle > 360)
    {
        targetAngle -= 360.0f;
        currentAngle -= 360.0f;
    }

    if (targetAngle < 360 && currentAngle < 360)
    {
        targetAngle += 360.0f;
        currentAngle += 360.0f;
    }

}

void GameState::checkLineCompletion() {

    float completedLine = -1.0f;
    std::map<float, std::vector<Block>> levelMap;

    // Group by y-level
    for (const auto& block : blocks) {
        levelMap[block.y].push_back(block);
    }

    for (auto it = levelMap.begin(); it != levelMap.end(); ++it) {
        float y = it->first;  
        const std::vector<Block>& blockList = it->second;

        if (blockList.size() != BOARD_SIZE * BOARD_SIZE) // line not completed,
            continue;

        completedLine = y; //find the lowest completed line since they get compeleted 3 lines at a time;
        break;
    }

    if (completedLine == -1.0f) return; 

    for (auto it = levelMap.begin(); it != levelMap.end(); ++it) { //lowering the y coordinates of the above ones;
        
        if (it->first < completedLine + 3.0f)
            continue;

        std::vector<Block>& blockList = it->second;

        for (Block& block : blockList)
        {
            block.y = round(block.y - 3.0f);
        }    
    }

    levelMap[completedLine].clear();
    levelMap[round(completedLine + 1.0f)].clear();
    levelMap[round(completedLine + 2.0f)].clear();

    blocks.clear();

    for (auto it = levelMap.begin(); it != levelMap.end(); ++it) {
        for (Block block : it->second)
            blocks.push_back(block);
    }

    score += 3 * BOARD_SIZE * BOARD_SIZE;


}
