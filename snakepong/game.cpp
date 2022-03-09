
#include "game.h"
#include "resource_manager.h"
#include "shader.h"

#include <iostream>
#include <algorithm>
#include <iterator>
#include <glm/gtx/string_cast.hpp>

// Shader *SnakeShader;
// Shader *SnakeSegmentShader;
// Shader *pongShader;

Game::Game(unsigned int width, unsigned int height)
    : State(GAME_SNAKE), Keys(), Width(width), Height(height),
      timeElapsed(0), paddleDir(0), paddleSpeed(10),
      SEG_SIZE(20), snakeMoveInterval(0.25f), snakeMoveTimer(0)
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

    // configure VAO/VBO
    // unsigned int VBO, quadVAO;

    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // printf("%d\n", sizeof(float) * numofVerts);

    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void *)0);
    glEnableVertexAttribArray(0);

    // Snake level size_x = WIDTH / SIZE
    //             size y = HEIGHT / SIZE
    snakeLvlSize = glm::ivec2(Width / SEG_SIZE, Height / SEG_SIZE);

    Reset();
}

void Game::Reset()
{
    this->snakeSegments.clear();
    this->snakeSegments.push_back(glm::ivec2(7, 5));
    this->snakeSegments.push_back(glm::ivec2(6, 5));
    this->snakeSegments.push_back(glm::ivec2(5, 5));
    this->applePos = glm::ivec2(9, 9);
    this->snakeDir = glm::ivec2(1, 0);
    this->paddleDir = 0;
    this->timeElapsed = 0;
    this->snakeMoveTimer = 0;
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
        if (this->Keys[GLFW_KEY_A] && snakeDir != glm::ivec2(1, 0))
        {
            this->snakeDir = glm::ivec2(-1, 0);
        }
        else if (this->Keys[GLFW_KEY_D] && snakeDir != glm::ivec2(-1, 0))
        {
            this->snakeDir = glm::ivec2(1, 0);
        }
        else if (this->Keys[GLFW_KEY_W] && snakeDir != glm::ivec2(0, 1))
        {
            this->snakeDir = glm::ivec2(0, -1);
        }
        else if (this->Keys[GLFW_KEY_S] && snakeDir != glm::ivec2(0, -1))
        {
            this->snakeDir = glm::ivec2(0, 1);
        }
    }
}

void Game::Update(float dt)
{
    this->timeElapsed += dt;
    this->snakeMoveTimer += dt;
    if (this->snakeMoveTimer > this->snakeMoveInterval)
    {
        // std::cout << "moving snake after " << snakeMoveTimer << " seconds" << std::endl;
        // for (auto e : snakeSegments)
        // {
        //     std::cout << e.x << ", " << e.y << ";  ";
        // }
        // std::cout << std::endl;
        snakeMoveTimer = 0;
        // Move snake
        glm::ivec2 oldfront = this->snakeSegments[0];
        // std::cout << "oldfront: " << glm::to_string(oldfront) << std::endl;
        // std::cout << "dir:   " << glm::to_string(snakeDir) << std::endl;
        this->snakeSegments.emplace(this->snakeSegments.begin(), glm::ivec2(oldfront.x + this->snakeDir.x, oldfront.y + this->snakeDir.y));
        // check for apple
        if (snakeSegments[0] != applePos)
            this->snakeSegments.pop_back();
        }
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

        // vertices for a square
        float squareVertices[] = {
            0.0f, 1.0f, // 1st tri
            1.0f, 0.0f,
            0.0f, 0.0f,
            0.0f, 1.0f, // 2nd tri
            1.0f, 1.0f,
            1.0f, 0.0f};

        float appleVertices[12];
        for (int j = 0; j < 12; j++)
        {
            appleVertices[j] = squareVertices[j];
        }
        glm::mat4 model = glm::mat4(1.0f);

        model = glm::scale(model, glm::vec3(SEG_SIZE, SEG_SIZE, 1.0f));
        model = glm::translate(model, glm::vec3((float)applePos.x, (float)applePos.y, 0.0f));

        // std::cout << "raw apple verts:\n";
        // for (int j = 0; j < 6; j++)
        // {
        //     std::cout << appleVertices[j] << ", " << appleVertices[j + 1] << "\n";
        // }
        // std::cout << std::endl;

        glm::vec4 vertTranslated;
        for (int j = 0; j < 6; j++)
        {
            glm::vec4 newv = glm::vec4(appleVertices[j * 2], appleVertices[j * 2 + 1], 0.0f, 1.0f);
            // std::cout << newv.x << ", " << newv.y << std::endl;

            vertTranslated = model * glm::vec4(appleVertices[j * 2], appleVertices[j * 2 + 1], 0.0f, 1.0f);
            // std::cout << vertTranslated.x << ", " << vertTranslated.y << std::endl;

            appleVertices[j * 2] = vertTranslated.x;
            appleVertices[j * 2 + 1] = vertTranslated.y;
        }
        // glfwTerminate();
        // exit(-1);

        glm::vec4 appleColor(1.0f, 0.0f, 0.0f, 1.0f);
        this->RenderVertices(this->SnakeShader, appleVertices, 6, appleColor);

        // 6 vertices per segment
        int numElements = snakeSegments.size() * 12;
        float vertices[numElements];

        // get vertices of snake to draw
        for (int i = 0; i < snakeSegments.size(); i++)
        {
            for (int j = 0; j < 12; j++)
            {
                vertices[i * 12 + j] = squareVertices[j];
            }
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::scale(model, glm::vec3(SEG_SIZE, SEG_SIZE, 1.0f));

            model = glm::translate(model, glm::vec3((float)snakeSegments[i].x, (float)snakeSegments[i].y, 0.0f));

            for (int j = 0; j < 6; j++)
            {
                glm::vec2 vertTranslated = model * glm::vec4(vertices[i * 12 + j * 2], vertices[i * 12 + j * 2 + 1], 0.0f, 1.0f);
                vertices[i * 12 + j * 2] = vertTranslated.x;
                vertices[i * 12 + j * 2 + 1] = vertTranslated.y;
            }
        }

        glm::vec4 snakeColor(0.0f, 1.0f, 0.0f, 1.0f);
        this->RenderVertices(this->SnakeShader, vertices, numElements / 2, snakeColor);
    }
}

void Game::RenderVertices(Shader shader, float verts[], int numofVerts, glm::vec4 color)
{

    // prepare transformations
    shader.Use();

    // glm::mat4 projection = glm::ortho(0.0f, 1200.0f, 900.0f, 0.0f, -1.0f, 1.0f);

    // std::cout << "model: " << glm::to_string(model * test) << std::endl;
    // std::cout << "ortho: " << glm::to_string(projection * model * test) << std::endl;

    // for (int i = 0; i < numofVerts * 2; i += 2)
    // {
    //     glm::vec4 test = glm::vec4(verts[i], verts[i + 1], 0.0f, 1.0f);
    //     std::cout << "model: " << glm::to_string(model * test) << std::endl;
    //     std::cout << "ortho: " << glm::to_string(projection * model * test) << std::endl;
    // }
    // printf("applepos %d,%d\n", applePos.x, applePos.y);

    // model = glm::translate(model, glm::vec3(0.5f * SEG_SIZE, 0.5f * SEG_SIZE, 0.0f));
    // model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f));
    // model = glm::translate(model, glm::vec3(-0.5f * SEG_SIZE, -0.5f * SEG_SIZE, 0.0f));

    // shader.SetMatrix4("model", model);
    shader.SetVector4f("spriteColor", color);

    // draw vertices

    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * numofVerts * 2, verts, GL_STATIC_DRAW);
    glBindVertexArray(this->quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, numofVerts);
    glBindVertexArray(0);
}
