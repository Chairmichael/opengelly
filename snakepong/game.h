/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
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

enum Direction
{
    STOP,
    RIGHT,
    UP,
    LEFT,
    DOWN
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
    // unsigned int quadVAO;
    Shader SnakeShader;

    // snake game 
    std::vector<glm::ivec2> snakeSegments;
    Direction snakeDir;
    glm::ivec2 applePos;
    const float SEG_SIZE; // size of segments and apple

    // pong game
    glm::vec2 paddlePos;
    int paddleDir;
    float paddleSpeed;

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