// filepath: /home/sunny_unix/graphHw3/main.cpp
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H
#include "game_state.h"
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

// Window dimensions
const int gWidth = 800;
const int gHeight = 600;

// Shader program IDs
GLuint gProgram[2];
GLint modelingMatrixLoc[2], viewingMatrixLoc[2], projectionMatrixLoc[2];
GLint eyePosLoc[2];

// Camera parameters
glm::vec3 eyePos(0.0f, 10.0f, 20.0f);
glm::mat4 projectionMatrix;
glm::mat4 viewingMatrix;
glm::mat4 modelingMatrix;

// Game state
GameState gameState;

// Shader loading functions
std::string readShaderFile(const char* fileName) {
    std::ifstream file(fileName);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

GLuint createShaderProgram(const char* vertexShaderSource, const char* fragmentShaderSource) {
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

void initShaders() {
    std::string vertexShader = readShaderFile("shaders/vertex.glsl");
    std::string fragmentShader = readShaderFile("shaders/fragment.glsl");
    
    // Create shader program
    gProgram[0] = createShaderProgram(vertexShader.c_str(), fragmentShader.c_str());
    
    // Get uniform locations
    modelingMatrixLoc[0] = glGetUniformLocation(gProgram[0], "modelingMatrix");
    viewingMatrixLoc[0] = glGetUniformLocation(gProgram[0], "viewingMatrix");
    projectionMatrixLoc[0] = glGetUniformLocation(gProgram[0], "projectionMatrix");
    eyePosLoc[0] = glGetUniformLocation(gProgram[0], "eyePos");
}

// Input handling
void keyboard(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action != GLFW_PRESS && action != GLFW_REPEAT) return;
    
    switch(key) {
        case GLFW_KEY_ESCAPE:
            glfwSetWindowShouldClose(window, GL_TRUE);
            break;
        case GLFW_KEY_A:
            gameState.moveBlock(-1);
            break;
        case GLFW_KEY_D:
            gameState.moveBlock(1);
            break;
        case GLFW_KEY_W:
            gameState.updateFallSpeed(-0.2f);
            break;
        case GLFW_KEY_S:
            gameState.updateFallSpeed(0.2f);
            break;
        case GLFW_KEY_H:
            gameState.rotateView(-90.0f);
            break;
        case GLFW_KEY_K:
            gameState.rotateView(90.0f);
            break;
    }
}

// Rendering functions
void drawCube(const glm::vec3& position, const glm::vec3& color) {
    modelingMatrix = glm::translate(glm::mat4(1.0f), position);
    glUniformMatrix4fv(modelingMatrixLoc[0], 1, GL_FALSE, glm::value_ptr(modelingMatrix));
    glUniform3fv(glGetUniformLocation(gProgram[0], "objectColor"), 1, glm::value_ptr(color));
    
    // Draw cube
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}

void drawBoard() {
    // Draw ground plane
    glm::vec3 boardColor(0.3f, 0.3f, 0.3f);
    for(int x = -4; x <= 4; x++) {
        for(int z = -4; x <= 4; z++) {
            drawCube(glm::vec3(x, -1, z), boardColor);
        }
    }
}

void drawBlocks() {
    glm::vec3 activeColor(1.0f, 0.0f, 0.0f);
    glm::vec3 placedColor(0.0f, 0.0f, 1.0f);
    
    // Draw active block
    for(const auto& block : gameState.activeBlock) {
        drawCube(glm::vec3(block.x, block.y, block.z), activeColor);
    }
    
    // Draw placed blocks
    for(const auto& block : gameState.blocks) {
        drawCube(glm::vec3(block.x, block.y, block.z), placedColor);
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Update view matrix based on rotation
    viewingMatrix = glm::rotate(
        glm::lookAt(eyePos, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)),
        glm::radians(gameState.viewRotation),
        glm::vec3(0, 1, 0)
    );
    
    glUseProgram(gProgram[0]);
    glUniformMatrix4fv(viewingMatrixLoc[0], 1, GL_FALSE, glm::value_ptr(viewingMatrix));
    
    drawBoard();
    drawBlocks();
}

void mainLoop(GLFWwindow* window) {
    float lastTime = glfwGetTime();
    
    while (!glfwWindowShouldClose(window)) {
        float currentTime = glfwGetTime();
        
        if (!gameState.gameOver) {
            gameState.updateBlockPosition(currentTime);
        }
        
        display();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

int main(int argc, char** argv) {
    if (!glfwInit()) {
        return -1;
    }
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    GLFWwindow* window = glfwCreateWindow(gWidth, gHeight, "tetrisGL", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }
    
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, keyboard);
    
    if (glewInit() != GLEW_OK) {
        return -1;
    }
    
    // Initialize OpenGL state
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    
    // Setup projection matrix
    projectionMatrix = glm::perspective(glm::radians(45.0f), 
                                      (float)gWidth/(float)gHeight, 
                                      0.1f, 100.0f);
    
    initShaders();
    
    mainLoop(window);
    
    glfwTerminate();
    return 0;
}