
#ifndef GAME_H
#define GAME_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>

#include "shader.h"
#include "resource_manager.h"

// Represents the current state of the game
enum GameState
{
    GAME_SNAKE,
    GAME_PONG,
    GAME_SNAKEPONG,
    GAME_SNAKE_WIN,
    GAME_PONG_WIN,
    GAME_LOSE
};

// Game holds all game-related state and functionality.
// Combines all game-related data into a single class for
// easy access to each of the components and manageability.
class Game
{
public:
    // game state
    GameState State;
    bool Keys[1024];
    unsigned int Width, Height;
    float timeElapsed;

    // rendering specific
    unsigned int VBO;
    unsigned int quadVAO;
    Shader SnakeShader;

    // snake game
    glm::ivec2 snakeLvlSize;
    std::vector<glm::ivec2> snakeSegments;
    glm::ivec2 snakeDir;
    glm::ivec2 snakeInputDir;
    float snakeMoveInterval; // number of times the snake is moved in snakeDir direction a second
    float snakeMoveTimer;
    glm::ivec2 applePos;
    const float SEG_SIZE; // size of segments and apple
    const float SEG_SCALE;

    // pong game
    glm::vec2 paddlePlayerPos, paddleNPPos, ballPos, ballVelocity;
    int paddleDir;
    float paddleSpeed, ballSpeed;
    const glm::vec3 PADDLE_SCALE, BALL_SCALE;

    // constructor/destructor
    Game(unsigned int width, unsigned int height);
    ~Game();
    // initialize game state (load all shaders/textures/levels)
    void Init();
    // game loop
    void ProcessInput(float dt);
    void Update(float dt);
    void Render();
    void RenderVertices(Shader shader, float verts[], int size, glm::vec4 color);
    void Reset();
};

#endif