/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#include "game.h"
#include "resource_manager.h"
#include "shader.h"

#include <iostream>
#include <glm/gtx/string_cast.hpp>

// Shader *SnakeShader;
// Shader *SnakeSegmentShader;
// Shader *pongShader;

Game::Game(unsigned int width, unsigned int height)
    : State(GAME_SNAKE), Keys(), Width(width), Height(height),
      timeElapsed(0), paddleDir(0), paddleSpeed(10), snakeDir(RIGHT),
      SEG_SIZE(20)
{
}

Game::~Game()
{
}

void Game::Init()
{
    // load shaders
    ResourceManager::LoadShader("simple.vs", "simple.fs", nullptr, "simple");
    // configure shaders
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->Width),
                                      static_cast<float>(this->Height), 0.0f, -1.0f, 1.0f);

    printf("width %f\n", static_cast<float>(this->Width));
    printf("height %f\n", static_cast<float>(this->Height));
    SnakeShader = ResourceManager::GetShader("simple");
    SnakeShader.SetMatrix4("projection", projection, true);
    printf("error %d\n", glGetError());

    this->Reset();
}

void Game::Reset()
{
    this->snakeSegments.push_back(glm::ivec2(5, 5));
    this->snakeSegments.push_back(glm::ivec2(6, 5));
    this->snakeSegments.push_back(glm::ivec2(7, 5));
    this->applePos = glm::ivec2(9, 9);
    this->snakeDir = RIGHT;
    this->paddleDir = 0;
    timeElapsed = 0;
}

void Game::ProcessInput(float dt)
{
    if (this->State == GAME_PONG)
    {
        // paddle movement
        if (this->Keys[GLFW_KEY_A])
        {
            this->paddleDir = -1; // Left, negative
        }
        if (this->Keys[GLFW_KEY_D])
        {
            this->paddleDir = 1; // Right, positive
        }
        if (this->Keys[GLFW_KEY_W])
        {
            this->paddleDir = 1; // Up, positive
        }
        if (this->Keys[GLFW_KEY_S])
        {
            this->paddleDir = -1; // Down, negative
        }
    }
    if (this->State == GAME_SNAKE)
    {
        // snake movement
        if (this->Keys[GLFW_KEY_A])
        {
            this->snakeDir = LEFT;
        }
        if (this->Keys[GLFW_KEY_D])
        {
            this->snakeDir = RIGHT;
        }
        if (this->Keys[GLFW_KEY_W])
        {
            this->snakeDir = UP;
        }
        if (this->Keys[GLFW_KEY_S])
        {
            this->snakeDir = DOWN;
        }
    }
}

void Game::Update(float dt)
{
}

void Game::Render()
{
    if (this->State == GAME_PONG)
    {
        // clear and fill background (rgba)
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    }
    else if (this->State == GAME_SNAKE)
    {
        // clear and fill background (rgba)
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // draw apple
        float appleVertices[] = {
            0.0f, 1.0f, // 1st tri
            1.0f, 0.0f,
            0.0f, 0.0f,
            0.0f, 1.0f, // 2nd tri
            1.0f, 1.0f,
            1.0f, 0.0f};

        glm::vec4 appleColor(1.0f, 0.0f, 0.0f, 1.0f);
        this->RenderVertices(this->SnakeShader, appleVertices, 6, appleColor);

        // an element for 6 vertices
        float vertices[snakeSegments.size() * 12];

        // get vertices of snake to draw
        for (glm::ivec2 pt : snakeSegments)
        {
        }
    }
}

void Game::RenderVertices(Shader shader, float verts[], int numofVerts, glm::vec4 color)
{
    // configure VAO/VBO
    unsigned int VBO, quadVAO;

    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // printf("%d\n", sizeof(float) * numofVerts);

    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * numofVerts * 2, verts, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void *)0);
    glEnableVertexAttribArray(0);

    // prepare transformations
    float rotate = 0.0f;
    shader.Use();
    glm::mat4 model = glm::mat4(1.0f);
    // want to: scale

    model = glm::scale(model, glm::vec3(SEG_SIZE, SEG_SIZE, 1.0f));
    model = glm::translate(model, glm::vec3((float)applePos.x, (float)applePos.y, 0.0f));

    glm::mat4 projection = glm::ortho(0.0f, 1200.0f, 900.0f, 0.0f, -1.0f, 1.0f);

    // std::cout << "model: " << glm::to_string(model * test) << std::endl;
    // std::cout << "ortho: " << glm::to_string(projection * model * test) << std::endl;

    for (int i = 0; i < numofVerts * 2; i += 2)
    {
        glm::vec4 test = glm::vec4(verts[i], verts[i + 1], 0.0f, 1.0f);
        std::cout << "model: " << glm::to_string(model * test) << std::endl;
        std::cout << "ortho: " << glm::to_string(projection * model * test) << std::endl;
    }
    // printf("applepos %d,%d\n", applePos.x, applePos.y);

    // model = glm::translate(model, glm::vec3(0.5f * SEG_SIZE, 0.5f * SEG_SIZE, 0.0f));
    // model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f));
    // model = glm::translate(model, glm::vec3(-0.5f * SEG_SIZE, -0.5f * SEG_SIZE, 0.0f));

    shader.SetMatrix4("model", model);
    shader.SetVector4f("spriteColor", color);

    // draw vertices
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, numofVerts);
    glBindVertexArray(0);
}
