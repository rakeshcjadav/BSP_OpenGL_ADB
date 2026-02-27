#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 TexCoord;
layout(location = 3) in vec2 ColorRange;
layout(location = 4) in vec3 Barycentric;

uniform mat4 uMatModel;
uniform mat4 uMatCamera;
uniform mat4 uMatProjection;

out OUT
{
    vec3 PositionFrag;
    vec3 NormalFrag;
    vec2 UVFrag;
    flat vec2 ColorRangeFrag;
    vec3 BarycentricFrag;
}vertOut;

void main()
{
    gl_Position = uMatProjection * uMatCamera * uMatModel * vec4(aPos, 1.0);
    vertOut.UVFrag = TexCoord;

    // Normal Direction of a vertex in World Space
    vertOut.NormalFrag = mat3(transpose(inverse(uMatModel))) * aNormal;
    //NormalFrag = normalize(NormalFrag);

    // Vertex Position in World Space
    vertOut.PositionFrag = vec3(uMatModel * vec4(aPos, 1.0));

    // Color Range for this vertex
    vertOut.ColorRangeFrag = ColorRange;
    
    // Barycentric coordinates for this vertex
    vertOut.BarycentricFrag = Barycentric;
};