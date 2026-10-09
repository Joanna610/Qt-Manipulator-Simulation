#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;

in vec4 vColor;
out vec4 fragColor;
void main()
{
    fragColor = vColor;
}
