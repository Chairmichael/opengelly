
#include "renderer2d.h"
#include <iostream>

Renderer2D::Renderer2D(Shader spriteShader, Shader simpleShader)
{
    this->spriteShader = spriteShader;
    this->simpleShader = simpleShader;
    this->initRenderData();
    std::cout << "constructed renderer" << std::endl;
}

Renderer2D::~Renderer2D()
{
    glDeleteVertexArrays(1, &this->quadVAO);
}

void Renderer2D::DrawSprite(Texture2D texture, glm::vec2 position, glm::vec2 size,
                            glm::vec3 color /* = glm::vec3(1.0f) */, float rotate /* = 0.0f */)
{
    // prepare transformations
    this->spriteShader.Use();
    glm::mat4 model = glm::mat4(1.0f);
    // first translate (transformations are: scale happens first, then rotation, and then final translation happens; reversed order)
    model = glm::translate(model, glm::vec3(position, 0.0f));

    model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));   // move origin of rotation to center of quad
    model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f));  // then rotate
    model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f)); // move origin back

    model = glm::scale(model, glm::vec3(size, 1.0f)); // last scale

    this->spriteShader.SetMatrix4("model", model);

    // render textured quad
    this->spriteShader.SetVector3f("spriteColor", color);

    glActiveTexture(GL_TEXTURE0);
    texture.Bind();

    glBindVertexArray(this->quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void Renderer2D::DrawQuad(glm::vec2 position, glm::vec2 size, glm::vec3 color, float rotate /* = 0.0f */)
{
    // prepare transformations
    this->simpleShader.Use();
    glm::mat4 model = glm::mat4(1.0f);
    // first translate (transformations are: scale happens first, then rotation, and then final translation happens; reversed order)
    model = glm::translate(model, glm::vec3(position, 0.0f));

    model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));   // move origin of rotation to center of quad
    model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f));  // then rotate
    model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f)); // move origin back

    model = glm::scale(model, glm::vec3(size, 1.0f)); // last scale

    this->simpleShader.SetMatrix4("model", model);
    this->simpleShader.SetVector3f("spriteColor", color);

    glBindVertexArray(this->quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void Renderer2D::initRenderData()
{
    // configure VAO/VBO
    unsigned int VBO;
    float vertices[] = {
        // pos      // tex
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f,

        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f};

    glGenVertexArrays(1, &this->quadVAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(this->quadVAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    std::cout << "init render data" << std::endl;
}

GLenum Renderer2D::CheckErrors_(const char *file, int line, bool waitOnError /* = true */, bool exitOnError /* = true */)
{
    GLenum errorCode;
    while ((errorCode = glGetError()) != GL_NO_ERROR)
    {
        std::cout << "GLerror in " << file << " (" << line << ") for ";
        switch (errorCode)
        {
        case GL_INVALID_ENUM:
            std::cout << "INVALID_ENUM:\n"
                      << "    Set when an enumeration parameter is not legal." << std::endl;
            break;
        case GL_INVALID_VALUE:
            std::cout << "INVALID_VALUE:\n"
                      << "    Set when a value parameter is not legal." << std::endl;
            break;
        case GL_INVALID_OPERATION:
            std::cout << "INVALID_OPERATION:\n"
                      << "    Set when the state for a command is not legal for its given parameters." << std::endl;
            break;
        case 0x0503:
            std::cout << "STACK_OVERFLOW:\n"
                      << "    Set when a stack pushing operation causes a stack overflow." << std::endl;
            break;
        case 0x0504:
            std::cout << "STACK_UNDERFLOW:\n"
                      << "    Set when a stack popping operation occurs while the stack is at its lowest point." << std::endl;
            break;
        case GL_OUT_OF_MEMORY:
            std::cout << "OUT_OF_MEMORY:\n"
                      << "    Set when a memory allocation operation cannot allocate (enough) memory." << std::endl;
            break;
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            std::cout << "INVALID_FRAMEBUFFER_OPERATION:\n"
                      << "    Set when reading or writing to a framebuffer that is not complete." << std::endl;
            break;
        }
        if (waitOnError)
        {
            std::cin;
        }
        if (exitOnError)
        {
            exit(1);
        }
    }
    return errorCode;
}
