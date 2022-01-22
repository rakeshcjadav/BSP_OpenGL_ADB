#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;

uniform mat4 uMatModel;
uniform mat4 uMatCamera;
uniform mat4 uMatProjection;

const float outline = 0.01f;
const mat3 uMatOutline = mat3(1.01f + outline, 0.0f, 0.0f,
                             0.0f, 1.01f + outline, 0.0f,
                             0.0f, 0.0f, 1.01f + outline);

out vec3 NormalFrag;

void main()
{
    // Model Space ( Local Space ) Scaling
    vec3 newPos = aPos + aNormal * outline;
    gl_Position = uMatProjection * uMatCamera * uMatModel * vec4(newPos, 1.0);
};