
#ifndef RENDERER2D_H
#define RENDERER2D_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "texture.h"
#include "shader.h"

class Renderer2D
{
public:
    // Constructor (inits shaders/shapes)
    Renderer2D(Shader spriteShader, Shader simpleShader);
    // Destructor
    ~Renderer2D();
    // Renders a defined quad textured with given sprite
    void DrawSprite(Texture2D &texture, glm::vec2 position,
                    glm::vec2 size = glm::vec2(10.0f, 10.0f), float rotate = 0.0f,
                    glm::vec3 color = glm::vec3(1.0f));
    //
    void DrawQuad(glm::vec2 position, glm::vec2 size, glm::vec3 color, float rotate = 0.0f);

private:
    // Render state
    Shader spriteShader;
    Shader simpleShader;
    unsigned int quadVAO;
    // Initializes and configures the quad's buffer and vertex attributes
    void initRenderData();
};

#endif