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
layout(location = 1) out vec4 Normal;

void main()
{
    // Ambient 
    // Directinal Light
    vec3 ambientColor = CalcAmbient(uDirectionalLight.color, uMaterial.ambient);
    // Point Light
    //float fAttenuation = CalcAttenuation(uPointLight.position, PositionFrag);
    //ambientColor += CalcAmbient(uPointLight.color, uMaterial.ambient) * fAttenuation;
    // Spot Light
    //fAttenuation = CalcAttenuation(uSpotLight.position, PositionFrag);
    //ambientColor += CalcAmbient(uSpotLight.color, uMaterial.ambient) * fAttenuation;

    vec3 normal = normalize(fragIn.NormalFrag);

    // Diffuse
    vec3 diffuseColor = vec3(0.0f);
    // Directinal Light
    diffuseColor += Diffuse(uDirectionalLight, normal);
    // Point Light
    diffuseColor += Diffuse(uPointLight, normal, fragIn.PositionFrag);
    // Spot Light
    diffuseColor += Diffuse(uSpotLight, normal, fragIn.PositionFrag);

    diffuseColor *= uMaterial.diffuse;

    // Specular ( Blin-Phong )
    // Directinal Light
    vec3 specularColor = Specular(uDirectionalLight, normal, fragIn.PositionFrag, uCameraPos, uMaterial);
    // Point Light
    specularColor += Specular(uPointLight, normal, fragIn.PositionFrag, uCameraPos, uMaterial);
    // Spot Light
    specularColor += Specular(uSpotLight, normal, fragIn.PositionFrag, uCameraPos, uMaterial);

    specularColor *= uMaterial.specular;

    vec3 finalColor = vec3(0.0f);
    finalColor += ambientColor;
    finalColor += diffuseColor;
    finalColor += specularColor;

    FragColor = vec4(finalColor, 1.0);
    vec3 normalColor;
    normalColor.r = remap(normal.r, -1.0, 1.0, 0.0, 1.0);
    normalColor.g = remap(normal.g, -1.0, 1.0, 0.0, 1.0);
    normalColor.b = remap(normal.b, -1.0, 1.0, 0.0, 1.0);
    Normal = vec4(normalColor, 1.0);
};