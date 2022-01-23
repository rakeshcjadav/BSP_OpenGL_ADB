#version 330 core
#include "common.glsl"

in vec2 UVFrag;

uniform sampler2D Texture;
uniform sampler2D Second;

layout(location = 0) out vec4 FragColor;
layout(location = 1) out vec4 Normal;

void main()
{
    vec2 scaledUV = (UVFrag - vec2(0.5)) * vec2(2.0) + vec2(0.5);
    vec2 scaledUV1 = UVFrag * vec2(2.0) - vec2(0.5);
    vec4 colorMinionOne = texture(Texture, UVFrag);
    vec4 colorMinionTwo = texture(Second, UVFrag);
    //vec4 colorMultiply = colorMinionOne* colorMinionTwo;
    //vec4 finalColor = colorMinionOne*colorMinionOne.a + colorMinionTwo*(1.0 - colorMinionOne.a);
    if(UVFrag.x < 0.5)
        FragColor = vec4(colorMinionTwo.rgb, 1.0);
    else
        FragColor = vec4(colorMinionOne.rgb, 1.0);

    Normal = vec4(1.0, 0.0f, 0.0f, 1.0);


};