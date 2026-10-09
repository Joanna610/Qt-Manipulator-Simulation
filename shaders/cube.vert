
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;

in vec3 aPosition;
in vec4 aNormal;
uniform mat4 uMvpMatrix;
uniform mat4 uNormalMatrix;
out vec4 vColor;
void main()
{
    gl_Position = uMvpMatrix * vec4(aPosition, 1.0);
    vec3 lightDirection = normalize(vec3(0.0, 0.5, 0.7));
    vec4 color = vec4(0.2, 0.2, 0.2, 1.0);
    vec3 normal = normalize((uNormalMatrix * aNormal).xyz);
    float nDotL = max(dot(normal, lightDirection), 0.0);
    vColor = vec4(color.rgb * nDotL + vec3(0.1), color.a);
}
