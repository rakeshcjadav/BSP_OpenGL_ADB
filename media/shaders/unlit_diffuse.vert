#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 2) in vec2 TexCoord;
//layout(location = 1) in vec3 Color;

uniform vec3 uColor;
uniform float uTime;
uniform mat4 uMatModel;
uniform mat4 uMatCamera;
uniform mat4 uMatProjection;

out vec2 UVFrag;

void main()
{
    gl_Position = uMatProjection * uMatCamera * uMatModel * vec4(aPos, 1.0);
    UVFrag = TexCoord;
};