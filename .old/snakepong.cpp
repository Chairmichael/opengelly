/**
 * snakepong.cpp
 * Jeff Henry (jeffvhenry@gmail.com)
 * A couple 2D opengl games
 * 2022-03-06
 */

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "game.h"

// GLFW function declarations
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode);

// The Width of the screen
const unsigned int SCREEN_WIDTH = 1200;
// The height of the screen
const unsigned int SCREEN_HEIGHT = 900;

Game Snakepong(SCREEN_WIDTH, SCREEN_HEIGHT);

int main(int argc, char *argv[])
{
    // Various configurations for opengl, glad and glfw
    // ------------------------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    glfwWindowHint(GLFW_RESIZABLE, false);

    GLFWwindow *window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "snakepong", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glfwSetKeyCallback(window, key_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // OpenGL configuration
    // --------------------
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // printf("error %d\n", glGetError());

    // initialize game
    // ---------------
    Snakepong.Init();
    // glfwTerminate();
    // return -1;

    // printf("error %d\n", glGetError());

    // deltaTime variables
    // -------------------
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    while (!glfwWindowShouldClose(window))
    {
        // printf("error %d\n", glGetError());
        // calculate delta time
        // --------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        glfwPollEvents();
        // printf("error %d\n", glGetError());
        // manage user input
        // -----------------
        Snakepong.ProcessInput(deltaTime);

        // printf("error %d\n", glGetError());
        // break;

        // update game state
        // -----------------
        Snakepong.Update(deltaTime);

        // render
        // ------
        Snakepong.Render();

        glfwSwapBuffers(window);
        // std::cin.get();
        // break;
    }

    // delete all resources as loaded using the resource manager
    // ---------------------------------------------------------
    ResourceManager::Clear();

    glfwTerminate();
    return 0;
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
    // when a user presses the escape key, we set the WindowShouldClose property to true, closing the application
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
            Snakepong.Keys[key] = true;
        else if (action == GLFW_RELEASE)
            Snakepong.Keys[key] = false;
    }
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}