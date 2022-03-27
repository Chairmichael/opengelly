
#include <algorithm>
#include <sstream>
#include <iostream>

#include "game.h"
#include "resource_manager.h"
#include "renderer2d.h"
#include "game_object.h"
#include "post_processor.h"

// Game-related State data
Renderer2D *Renderer;
// PostProcessor *Effects;

Game::Game(unsigned int width, unsigned int height)
    : State(GAME_MENU), Keys(), KeysProcessed(), Width(width), Height(height)
{
}

Game::~Game()
{
    delete Renderer;
}

void Game::Init()
{
    // load shaders
    ResourceManager::LoadShader("shaders/simple.vs", "shaders/simple.fs", nullptr, "simple");
    ResourceManager::LoadShader("shaders/sprite.vs", "shaders/sprite.fs", nullptr, "sprite");
    // ResourceManager::LoadShader("post_processing.vs", "post_processing.fs", nullptr, "postprocessing");

    // configure shaders
    glm::mat4 projection = glm::ortho(0.0f, (float)this->Width, (float)this->Height, 0.0f, -1.0f, 1.0f);
    ResourceManager::GetShader("simple").Use().SetInteger("simple", 0);
    ResourceManager::GetShader("simple").SetMatrix4("projection", projection);
    ResourceManager::GetShader("sprite").Use().SetInteger("sprite", 0);
    ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);

    // set render-specific controls
    Renderer = new Renderer2D(ResourceManager::GetShader("sprite"), ResourceManager::GetShader("simple"));

    // Effects = new PostProcessor(ResourceManager::GetShader("postprocessing"), this->Width, this->Height);
    // load textures
    ResourceManager::LoadTexture("assets/awesomeface.png", true, "face");
    // configure game objects
}

void Game::Update(float dt)
{
    // update objects

    // check loss condition

    // check win condition
}

void Game::ProcessInput(float dt)
{
    if (this->State == GAME_MENU)
    {
    }
    if (this->State == GAME_WIN)
    {
    }
    if (this->State == GAME_ACTIVE)
    {
    }
}

void Game::Render()
{
    if (this->State == GAME_ACTIVE || this->State == GAME_MENU || this->State == GAME_WIN)
    {
        Renderer->DrawQuad(glm::vec2(0.0f, 0.0f), glm::vec2(100.0f, 100.0f), glm::vec3(1.0f, 1.0f, 1.0f));
        Renderer->DrawSprite(ResourceManager::GetTexture("face"),
                             glm::vec2(200.0f, 200.0f), glm::vec2(300.0f, 400.0f), glm::vec3(0.0f, 1.0f, 0.0f), 45.0f);
    }
    if (this->State == GAME_MENU)
    {
    }
    if (this->State == GAME_WIN)
    {
    }
}

void Game::Reset()
{
}
