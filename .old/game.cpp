
#include "game.h"
#include "resource_manager.h"
#include "shader.h"

#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <iterator>
#include <glm/gtx/string_cast.hpp>

// Shader *SnakeShader;
// Shader *SnakeSegmentShader;
// Shader *pongShader;

Game::Game(unsigned int width, unsigned int height)
    : State(GAME_SNAKE), Keys(), Width(width), Height(height),
      timeElapsed(0), paddleDir(0), paddleSpeed(12), ballSpeed(500),
      SEG_SIZE(60), SEG_SCALE(0.9f), snakeMoveInterval(0.15f), snakeMoveTimer(0),
      PADDLE_SCALE(SEG_SIZE * 3, SEG_SIZE / 5, 1.0f), BALL_SCALE(SEG_SIZE / 2, SEG_SIZE / 2, 1.0f)
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
    snakeLvlSize = glm::ivec2(Width / (int)SEG_SIZE, Height / (int)SEG_SIZE);

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
    this->snakeInputDir = snakeDir;
    this->timeElapsed = 0;
    this->snakeMoveTimer = 0;
    srand(time(0)); // seed rng
    applePos.x = rand() % snakeLvlSize.x;
    applePos.y = rand() % snakeLvlSize.y;

    paddleDir = 0;
    paddlePlayerPos = glm::vec2(Width / 2, Height - PADDLE_SCALE.y * 2);
    paddleNPPos = glm::vec2(Width / 2, PADDLE_SCALE.y * 1.5f);
    float randx = rand() % 100;
    ballPos = glm::vec2(Width * (0.05f + 0.9f * randx / 100.0f), Height * 0.2f);

    float randa = (rand() % 100) / 100.0f * 2 * glm::pi<float>() / 3; // between 0 and 2*PI/3
    glm::vec2 newDir = glm::vec2(cos(randa + 2 * glm::pi<float>() / 6), sin(randa + 2 * glm::pi<float>() / 6));
    ballVelocity = ballSpeed * glm::normalize(newDir);
}

void Game::ProcessInput(float dt)
{
    if (this->Keys[GLFW_KEY_A])
    {
        if (snakeDir != glm::ivec2(1, 0))
            this->snakeInputDir = glm::ivec2(-1, 0);
        paddleDir = -1;
    }
    else if (this->Keys[GLFW_KEY_D])
    {
        if (snakeDir != glm::ivec2(-1, 0))
            this->snakeInputDir = glm::ivec2(1, 0);
        paddleDir = 1;
    }
    else if (this->Keys[GLFW_KEY_W] && snakeDir != glm::ivec2(0, 1))
    {
        this->snakeInputDir = glm::ivec2(0, -1);
    }
    else if (this->Keys[GLFW_KEY_S] && snakeDir != glm::ivec2(0, -1))
    {
        this->snakeInputDir = glm::ivec2(0, 1);
    }
    else
    {
        paddleDir = 0;
    }
}

void Game::Update(float dt)
{
    this->timeElapsed += dt;
    this->snakeMoveTimer += dt;
    if (this->snakeMoveTimer > this->snakeMoveInterval)
    {
        // snake movement from input "buffer"
        snakeDir = snakeInputDir;
        snakeMoveTimer = 0;
        // Move snake
        glm::ivec2 oldfront = this->snakeSegments[0];
        // std::cout << "oldfront: " << glm::to_string(oldfront) << std::endl;
        // std::cout << "dir:   " << glm::to_string(snakeDir) << std::endl;
        this->snakeSegments.emplace(this->snakeSegments.begin(),
                                    glm::ivec2(oldfront.x + this->snakeDir.x, oldfront.y + this->snakeDir.y));

        // check for apple
        if (snakeSegments[0] != applePos)
            this->snakeSegments.pop_back();
        else
        {
            // ate apple
            applePos.x = rand() % snakeLvlSize.x;
            applePos.y = rand() % snakeLvlSize.y;
        }
        // check for snake bounds and collisions
        glm::ivec2 snakeHead = snakeSegments[0];
        for (int i = 1; i < snakeSegments.size(); i++)
        {
            if (snakeHead == snakeSegments[i])
            {
                std::cout << "Game over!\nResetting..." << std::endl;
                this->Reset();
            }
        }
        if (snakeHead.x < 0 || snakeHead.x >= snakeLvlSize.x ||
            snakeHead.y < 0 || snakeHead.y >= snakeLvlSize.y)
        {
            std::cout << "Game over!\nResetting..." << std::endl;
            this->Reset();
        }
    }
    // pong updates
    // ball collides with side
    if (ballPos.x - BALL_SCALE.x / 2 < 0 || ballPos.x + BALL_SCALE.x / 2 > Width)
        ballVelocity.x *= -1.0f;
    // // non player paddle has collided with ball
    if (ballPos.y - BALL_SCALE.y / 2 < paddleNPPos.y + PADDLE_SCALE.y / 2)
    {
        ballVelocity.y *= -1.0f;
    }
    else if (ballPos.y + BALL_SCALE.y / 2 > paddlePlayerPos.y - PADDLE_SCALE.y / 2)
    {
        float diff = (paddlePlayerPos.x - ballPos.x + PADDLE_SCALE.x / 2) / PADDLE_SCALE.x;
        if (diff < 1 && diff > 0)
        {
            std::cout << diff << std::endl;
            float newAngle = diff * glm::pi<float>();
            ballVelocity = -1 * ballSpeed * glm::vec2(glm::cos(newAngle), glm::sin(newAngle));
            // ballVelocity.y *= -1.0f;
        }
        else
        {
            std::cout << "Game over!\nResetting..." << std::endl;
            this->Reset();
        }
    }
    else if (ballPos.y - BALL_SCALE.y / 2 < paddleNPPos.y + PADDLE_SCALE.y / 2)
    {
        // player wins
        std::cout << "You win!\nResetting..." << std::endl;
        this->Reset();
    }
    // update ball
    ballPos += dt * ballVelocity;
    paddlePlayerPos.x += paddleDir * paddleSpeed;
    paddleNPPos.x = std::clamp(ballPos.x, PADDLE_SCALE.x / 2, Width - PADDLE_SCALE.x / 2);
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

        // Compute apple vertices
        // ----------------------
        float appleVertices[12];
        for (int j = 0; j < 12; j++)
        {
            appleVertices[j] = squareVertices[j];
        }
        glm::mat4 model = glm::mat4(1.0f);

        //
        model = glm::scale(model, glm::vec3(SEG_SIZE, SEG_SIZE, 1.0f));
        model = glm::translate(model, glm::vec3((float)applePos.x, (float)applePos.y, 0.0f));

        // scale down
        model = glm::scale(model, glm::vec3(SEG_SCALE, SEG_SCALE, 1.0f));
        model = glm::translate(model, glm::vec3((1 - SEG_SCALE) / 2, (1 - SEG_SCALE) / 2, 0.0f));

        glm::vec4 vertTranslated;
        for (int j = 0; j < 6; j++)
        {
            glm::vec4 newv = glm::vec4(appleVertices[j * 2], appleVertices[j * 2 + 1], 0.0f, 1.0f);

            vertTranslated = model * glm::vec4(appleVertices[j * 2], appleVertices[j * 2 + 1], 0.0f, 1.0f);

            appleVertices[j * 2] = vertTranslated.x;
            appleVertices[j * 2 + 1] = vertTranslated.y;
        }

        glm::vec4 appleColor(1.0f, 0.0f, 0.0f, 1.0f);
        this->RenderVertices(this->SnakeShader, appleVertices, 6, appleColor);

        // Compute snake vertices
        // ----------------------
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

            // scale down
            if (i == 0)
                model = glm::scale(model, glm::vec3(SEG_SCALE, SEG_SCALE, 1.0f));
            else
                model = glm::scale(model, glm::vec3(SEG_SCALE, SEG_SCALE, 1.0f));
            model = glm::translate(model, glm::vec3((1 - SEG_SCALE) / 2, (1 - SEG_SCALE) / 2, 0.0f));

            for (int j = 0; j < 6; j++)
            {
                glm::vec2 vertTranslated = model * glm::vec4(vertices[i * 12 + j * 2], vertices[i * 12 + j * 2 + 1], 0.0f, 1.0f);
                vertices[i * 12 + j * 2] = vertTranslated.x;
                vertices[i * 12 + j * 2 + 1] = vertTranslated.y;
            }
        }

        glm::vec4 snakeColor(0.0f, 1.0f, 0.0f, 1.0f);
        this->RenderVertices(this->SnakeShader, vertices, numElements / 2, snakeColor);

        // Compute pong vertices
        // ---------------------
        float pongVerts[36];
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 12; j++)
                pongVerts[i * 12 + j] = squareVertices[j];

            if (i == 0)
            {
                // ball
                model = glm::mat4(1.0f);
                model = glm::translate(model, glm::vec3(BALL_SCALE.x * -0.5f + ballPos.x,
                                                        BALL_SCALE.y * -0.5f + ballPos.y, 0.0f));
                model = glm::scale(model, BALL_SCALE);

                for (int j = 0; j < 6; j++)
                {
                    glm::vec2 vertTranslated = model * glm::vec4(pongVerts[i * 12 + j * 2], pongVerts[i * 12 + j * 2 + 1], 0.0f, 1.0f);
                    // pongVerts[i * 12 + j * 2] = 0;
                    // pongVerts[i * 12 + j * 2 + 1] = 0;
                    pongVerts[i * 12 + j * 2] = vertTranslated.x;
                    pongVerts[i * 12 + j * 2 + 1] = vertTranslated.y;
                }
            }
            else if (i == 1)
            {
                // players paddle
                model = glm::mat4(1.0f);
                model = glm::translate(model, glm::vec3(paddlePlayerPos.x - PADDLE_SCALE.x * 0.5f,
                                                        paddlePlayerPos.y, 0.0f));
                model = glm::scale(model, PADDLE_SCALE);

                for (int j = 0; j < 6; j++)
                {
                    glm::vec2 vertTranslated = model * glm::vec4(pongVerts[i * 12 + j * 2], pongVerts[i * 12 + j * 2 + 1], 0.0f, 1.0f);
                    pongVerts[i * 12 + j * 2] = vertTranslated.x;
                    pongVerts[i * 12 + j * 2 + 1] = vertTranslated.y;
                    // std::cout << glm::to_string(vertTranslated) << std::endl;
                }
            }
            else if (i == 2)
            {
                // non players paddle
                model = glm::mat4(1.0f);
                model = glm::translate(model, glm::vec3(paddleNPPos.x - PADDLE_SCALE.x * 0.5f,
                                                        paddleNPPos.y - PADDLE_SCALE.y, 0.0f));
                model = glm::scale(model, PADDLE_SCALE);

                for (int j = 0; j < 6; j++)
                {
                    glm::vec2 vertTranslated = model * glm::vec4(pongVerts[i * 12 + j * 2], pongVerts[i * 12 + j * 2 + 1], 0.0f, 1.0f);
                    pongVerts[i * 12 + j * 2] = vertTranslated.x;
                    pongVerts[i * 12 + j * 2 + 1] = vertTranslated.y;
                    // std::cout << glm::to_string(vertTranslated) << std::endl;
                }
            }
        }

        // std::cout << "\npongVerts:\n";
        // for (int i = 0; i < 36; i++)
        // {
        //     std::cout << pongVerts[i] << " ";
        //     if (i % 2 == 0 && i != 0)
        //         std::cout << "\n";
        //     if (i % 12 == 0)
        //         std::cout << "\n";
        // }
        // std::cout << std::endl;
        // std::cin.get();

        glm::vec4 pongColor(1.0f, 1.0f, 1.0f, 1.0f);
        this->RenderVertices(this->SnakeShader, pongVerts, 36, pongColor);
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
