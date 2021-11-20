#version 330 core

in vec3 colorFrag;
in vec2 UVFrag;

uniform vec3 uColor[2];
uniform sampler2D Texture;
uniform sampler2D Second;

out vec4 FragColor;

void main()
{
    vec2 scaledUV = (UVFrag - vec2(0.5)) * vec2(2.0) + vec2(0.5);
    vec2 scaledUV1 = UVFrag * vec2(2.0) - vec2(0.5);
    vec4 colorMinionOne = texture(Texture, UVFrag);
    //vec4 colorMinionTwo = texture(Second, UVFrag);
    //vec4 colorMultiply = colorMinionOne* colorMinionTwo;
    //vec4 finalColor = colorMinionOne*colorMinionOne.a + colorMinionTwo*(1.0 - colorMinionOne.a);
    FragColor = colorMinionOne;
};