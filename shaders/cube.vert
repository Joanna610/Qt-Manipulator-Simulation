#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;

uniform mat4 uMvpMatrix;
uniform mat4 uNormalMatrix;

out vec3 vNormal;

void main()
{
    gl_Position = uMvpMatrix * vec4(position, 1.0);

    vNormal = mat3(uNormalMatrix) * normal;
}