#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 TexCoord;

uniform mat4 uMatModel;
uniform mat4 uMatCamera;
uniform mat4 uMatProjection;

out vec3 PositionFrag;
out vec3 NormalFrag;
out vec2 UVFrag;

void main()
{
    gl_Position = uMatProjection * uMatCamera * uMatModel * vec4(aPos, 1.0);
    UVFrag = TexCoord;

    // Normal Direction of a vertex in World Space
    NormalFrag = mat3(transpose(inverse(uMatModel))) * aNormal;

    // Vertex Position in World Space
    PositionFrag = vec3(uMatModel * vec4(aPos, 1.0));
};