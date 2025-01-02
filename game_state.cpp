#include "game_state.h"
#include <algorithm>

const float GameState::INITIAL_FALL_SPEED = 1.0f;
const float GameState::MAX_FALL_SPEED = 5.0f;
const float GameState::MIN_FALL_SPEED = 0.5f;

GameState::GameState() {
    fallSpeed = INITIAL_FALL_SPEED;
    lastFallTime = 0.0f;
    score = 0;
    gameOver = false;
    viewRotation = 0.0f;
    createNewBlock();
}

void GameState::createNewBlock() {
    activeBlock.clear();
    // Create 3x3 block at top center
    for(int x = -1; x <= 1; x++) {
        for(int y = 0; y < 3; y++) {
            for(int z = -1; z <= 1; z++) {
                Block block = {
                    static_cast<float>(x),
                    12.0f + static_cast<float>(y),
                    static_cast<float>(z),
                    true
                };
                activeBlock.push_back(block);
            }
        }
    }
}

void GameState::updateBlockPosition(float currentTime) {
    if (currentTime - lastFallTime >= 1.0f/fallSpeed) {
        // Move block down
        for(auto& block : activeBlock) {
            block.y -= 1.0f;
        }
        
        if(checkCollision()) {
            // Move block back up
            for(auto& block : activeBlock) {
                block.y += 1.0f;
            }
            // Add to placed blocks
            blocks.insert(blocks.end(), activeBlock.begin(), activeBlock.end());
            checkLineCompletion();
            createNewBlock();
            if(checkCollision()) {
                gameOver = true;
            }
        }
        lastFallTime = currentTime;
    }
}

bool GameState::checkCollision() {
    // Check ground collision
    for(const auto& block : activeBlock) {
        if(block.y < 0) return true;
        
        // Check collision with placed blocks
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

void GameState::moveBlock(int direction) {
    // Move all blocks
    for(auto& block : activeBlock) {
        block.x += direction;
    }
    
    // Check boundaries and collisions
    if(checkCollision()) {
        // Move back if invalid
        for(auto& block : activeBlock) {
            block.x -= direction;
        }
    }
}