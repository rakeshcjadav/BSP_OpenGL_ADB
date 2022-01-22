#version 330 core
#include "common.glsl"

uniform TexturedMaterial uMaterial;

out vec4 FragColor;

void main()
{
    FragColor = vec4(uMaterial.material.diffuse, 1.0);
}