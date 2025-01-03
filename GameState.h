#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <vector>
#include <glm/glm.hpp>

struct Block {
    float x, y, z;
    bool active;
};

enum Direction
{
    Front = 0,
    Right = 1,
    Back = 2,
    Left = 3
};

class GameState {
public:
    GameState();

    // Game state
    float fallSpeed;
    float lastFallTime;
    int score;
    bool gameOver;
    Direction facedDirection;

    // Block management
    std::vector<Block> blocks;
    std::vector<Block> activeBlock;

    // Game functions
    void createNewBlock();
    void updateBlockPosition(float currentTime);
    bool checkCollision();
    bool checkBoundariesCollision();
    void moveBlock(int direction); // -1 left, 1 right
    void updateFallSpeed(float delta);
    void rotateView(int direction); // -1 left, 1 right
    void checkLineCompletion();

    // Constants
    static const int BOARD_SIZE = 9;
    static const int BLOCK_SIZE = 3;
    static const float INITIAL_FALL_SPEED;
    static const float MAX_FALL_SPEED;
    static const float MIN_FALL_SPEED;
};

#endif