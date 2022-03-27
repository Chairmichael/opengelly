
#ifndef RENDERER2D_H
#define RENDERER2D_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "texture.h"
#include "shader.h"

#define GLCheckErrors() Renderer2D::CheckErrors_(__FILE__, __LINE__, true, true)
// #define GLCheckErrors(waitOnError) Renderer2D::CheckErrors_(__FILE__, __LINE__, waitOnError, true)
// #define GLCheckErrors(waitOnError, exitOnError) Renderer2D::CheckErrors_(__FILE__, __LINE__, waitOnError, exitOnError)

class Renderer2D
{
public:
    // Constructor (inits shaders/shapes)
    Renderer2D(Shader spriteShader, Shader simpleShader);
    // Destructor
    ~Renderer2D();
    // Renders a defined quad textured with given sprite
    void DrawSprite(Texture2D texture, glm::vec2 position, glm::vec2 size, glm::vec3 color = glm::vec3(1.0f),
                    float rotate = 0.0f);
    // Renders a quad with a color
    void DrawQuad(glm::vec2 position, glm::vec2 size, glm::vec3 color, float rotate = 0.0f);
    // Checks for errors
    static GLenum CheckErrors_(const char *file, int line, bool waitOnError = true, bool exitOnError = true);

private:
    // Render state
    Shader spriteShader;
    Shader simpleShader;
    unsigned int quadVAO;
    // Initializes and configures the quad's buffer and vertex attributes
    void initRenderData();
};

#endif