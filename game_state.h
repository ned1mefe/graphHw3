#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <vector>
#include <glm/glm.hpp>

struct Block {
    float x, y, z;
    bool active;
};

class GameState {
public:
    GameState();
    
    // Game state
    float fallSpeed;
    float lastFallTime;
    int score;
    bool gameOver;
    float viewRotation;
    
    // Block management
    std::vector<Block> blocks;
    std::vector<Block> activeBlock;
    
    // Game functions
    void createNewBlock();
    void updateBlockPosition(float currentTime);
    bool checkCollision();
    void moveBlock(int direction); // -1 left, 1 right
    void updateFallSpeed(float delta);
    void rotateView(float angle);
    void checkLineCompletion();
    
    // Constants
    static const int BOARD_SIZE = 9;
    static const int BLOCK_SIZE = 3;
    static const float INITIAL_FALL_SPEED;
    static const float MAX_FALL_SPEED;
    static const float MIN_FALL_SPEED;
};

#endif