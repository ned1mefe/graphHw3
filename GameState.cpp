#include "GameState.h"
#include <algorithm>
#include <random>
#include <map>
#include <cmath>
//#define M_PI acos(-1)

const float GameState::INITIAL_FALL_SPEED = 1.0f;
const float GameState::MIN_FALL_SPEED = 0.5f;
const float GameState::MAX_FALL_SPEED = 3.0f;

GameState::GameState() :
    fallSpeed(INITIAL_FALL_SPEED),
    lastFallTime(0.0f),
    score(0),
    gameOver(false),
    viewRotation(0.0f) {
    createNewBlock();
}

void GameState::createNewBlock() {
    activeBlock.clear();

    for (int x = 3; x < 6; x++) {
        for (int y = 12; y < 15; y++) {
            for (int z = 3; z <= 6; z++) {
                Block block = {
                    static_cast<float>(x),
                    static_cast<float>(y),
                    static_cast<float>(z),
                    true
                };
                activeBlock.push_back(block);
            }
        }
    }

    if (checkCollision()) {
        gameOver = true;
    }
}

void GameState::updateBlockPosition(float currentTime) {
    if (currentTime - lastFallTime >= 1.0f / fallSpeed) {
        // Move block down
        for (auto& block : activeBlock) {
            block.y -= 1.0f;
        }

        if (checkCollision()) {
            // Move block back up
            for (auto& block : activeBlock) {
                block.y += 1.0f;
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

    //sadece bakis acisini degistirdigimiz icin x i guncellememiz yeterli

    for (auto& block : activeBlock) {
        block.x += direction;
    }

    if (checkCollision() || checkBoundariesCollision()) {
        for (auto& block : activeBlock) {
            block.x -= direction;
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

void GameState::rotateView(float angle) {

    //TAMAMEN YANLIÞ KÜPLERÝ DÖNDÜRMEMÝZ GEREKIYOR BAKIÞ ACISINI DEÐÝL

    /*viewRotation += angle;
    while (viewRotation >= 360.0f) viewRotation -= 360.0f;
    while (viewRotation < 0.0f) viewRotation += 360.0f;*/
}

void GameState::checkLineCompletion() {
    //std::map<float, std::vector<Block>> levelMap;

    //// Group by y-level
    //for (const auto& block : blocks) {
    //    levelMap[block.y].push_back(block);
    //}

    //for (const auto& [level, levelBlocks] : levelMap) {
    //    if (levelBlocks.size() == BOARD_SIZE * BOARD_SIZE) {
    //        // Remove completed level
    //        blocks.erase(
    //            std::remove_if(blocks.begin(), blocks.end(),
    //                [level](const Block& b) { return b.y == level; }
    //            ),
    //            blocks.end()
    //        );

    //        // Move blocks down
    //        for (auto& block : blocks) {
    //            if (block.y > level) {
    //                block.y -= 1.0f;
    //            }
    //        }

    //        score += BOARD_SIZE * BOARD_SIZE;
    //    }
    //}
}