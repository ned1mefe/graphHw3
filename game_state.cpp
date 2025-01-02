#include "game_state.h"
#include <algorithm>
#include <random>
#include <map>
#include <cmath>

const float GameState::INITIAL_FALL_SPEED = 1.0f;
const float MIN_FALL_SPEED = 0.5f;
const float MAX_FALL_SPEED = 3.0f;

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
    
    // Create 3x3x3 cube at top-center
    for(int x = -1; x <= 1; x++) {
        for(int y = 20; y <= 22; y++) {
            for(int z = -1; z <= 1; z++) {
                activeBlock.push_back({
                    static_cast<float>(x),
                    static_cast<float>(y),
                    static_cast<float>(z),
                    true
                });
            }
        }
    }
    
    if(checkCollision()) {
        gameOver = true;
    }
}

bool GameState::checkCollision() {
    for(const auto& block : activeBlock) {
        // Floor collision
        if(block.y < 0) return true;
        
        // Wall boundaries
        if(block.x < -(BOARD_SIZE/2) || block.x >= (BOARD_SIZE/2) ||
           block.z < -(BOARD_SIZE/2) || block.z >= (BOARD_SIZE/2)) {
            return true;
        }
        
        // Block collisions
        for(const auto& placedBlock : blocks) {
            if(block.x == placedBlock.x && 
               block.y == placedBlock.y && 
               block.z == placedBlock.z) {
                return true;
            }
        }
    }
    return false;
}

void GameState::updateBlockPosition(float currentTime) {
    if(currentTime - lastFallTime >= 1.0f / fallSpeed) {
        // Move down
        for(auto& block : activeBlock) {
            block.y -= 1.0f;
        }
        
        if(checkCollision()) {
            // Revert and place block
            for(auto& block : activeBlock) {
                block.y += 1.0f;
            }
            blocks.insert(blocks.end(), activeBlock.begin(), activeBlock.end());
            checkLineCompletion();
            createNewBlock();
        }
        
        lastFallTime = currentTime;
    }
}

void GameState::moveBlock(int direction) {
    float dx = 0.0f, dz = 0.0f;
    
    // Apply view rotation to movement
    float angle = viewRotation * M_PI / 180.0f;
    dx = direction * cos(angle);
    dz = -direction * sin(angle);
    
    // Move blocks
    for(auto& block : activeBlock) {
        block.x += dx;
        block.z += dz;
    }
    
    if(checkCollision()) {
        // Revert if invalid
        for(auto& block : activeBlock) {
            block.x -= dx;
            block.z -= dz;
        }
    }
}

void GameState::updateFallSpeed(float delta) {
    fallSpeed = std::clamp(fallSpeed + delta, MIN_FALL_SPEED, MAX_FALL_SPEED);
}

void GameState::rotateView(float angle) {
    viewRotation += angle;
    while(viewRotation >= 360.0f) viewRotation -= 360.0f;
    while(viewRotation < 0.0f) viewRotation += 360.0f;
}

void GameState::checkLineCompletion() {
    std::map<float, std::vector<Block>> levelMap;
    
    // Group by y-level
    for(const auto& block : blocks) {
        levelMap[block.y].push_back(block);
    }
    
    for(const auto& [level, levelBlocks] : levelMap) {
        if(levelBlocks.size() == BOARD_SIZE * BOARD_SIZE) {
            // Remove completed level
            blocks.erase(
                std::remove_if(blocks.begin(), blocks.end(),
                    [level](const Block& b) { return b.y == level; }
                ),
                blocks.end()
            );
            
            // Move blocks down
            for(auto& block : blocks) {
                if(block.y > level) {
                    block.y -= 1.0f;
                }
            }
            
            score += BOARD_SIZE * BOARD_SIZE;
        }
    }
}