#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 TexCoord;
layout(location = 3) in vec2 ColorRange;
layout(location = 4) in vec3 Barycentric;
layout(location = 5) in vec3 Displacement;

uniform mat4 uMatModel;
uniform mat4 uMatCamera;
uniform mat4 uMatProjection;
uniform float uDisplacementScale;

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
    vec3 displacedPos = aPos + Displacement * uDisplacementScale;
    gl_Position = uMatProjection * uMatCamera * uMatModel * vec4(displacedPos, 1.0);
    vertOut.UVFrag = TexCoord;

    vertOut.NormalFrag = mat3(transpose(inverse(uMatModel))) * aNormal;

    vertOut.PositionFrag = vec3(uMatModel * vec4(displacedPos, 1.0));

    vertOut.ColorRangeFrag = ColorRange;
    
    vertOut.BarycentricFrag = Barycentric;
};