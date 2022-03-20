#version 330 core

out vec4 color;
uniform vec4 spriteColor;

void main()
{
    color = spriteColor;
    // color = vec4(1.0f, 0.0f, 0.0f, 1.0f);
}