#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;

uniform float offset;

out vec3 ourColor;
out vec4 newPos;

void main()
{
    newPos = vec4(aPos.x * -1.0 + offset, aPos.y * -1.0, aPos.z * -1.0, 1.0);
    gl_Position = vec4(newPos);
    ourColor = aColor;
}