#version 330 core
#include "common.glsl"

in OUT
{
    // Fragment Position in World Space
    vec3 PositionFrag;
    // Normal Direction of a fragment in World Space
    vec3 NormalFrag;
    vec2 UVFrag;
}fragIn;

uniform Material uMaterial;
uniform DirectionalLight uDirectionalLight;
uniform PointLight uPointLight;
uniform SpotLight uSpotLight;
uniform vec3 uCameraPos;

layout(location = 0) out vec4 FragColor;

void main()
{
    // Ambient 
    // Directinal Light
    vec3 ambientColor = CalcAmbient(uDirectionalLight.color, uMaterial.ambient);

    vec3 normal = normalize(fragIn.NormalFrag);

    // Diffuse
    vec3 diffuseColor = vec3(0.0f);
    // Directinal Light
    diffuseColor += Diffuse(uDirectionalLight, normal);

    // Specular ( Blin-Phong )
    // Directinal Light
    vec3 specularColor = Specular(uDirectionalLight, normal, fragIn.PositionFrag, uCameraPos, uMaterial);

    specularColor *= uMaterial.specular;

    vec3 finalColor = vec3(0.0f);
    finalColor += ambientColor;
    finalColor += diffuseColor;
    finalColor += specularColor;

    FragColor = vec4(finalColor, 1.0);
};