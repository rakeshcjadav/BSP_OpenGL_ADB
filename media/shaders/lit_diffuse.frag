#version 330 core
#include "common.glsl"

// Fragment Position in World Space
in vec3 PositionFrag;
// Normal Direction of a fragment in World Space
in vec3 NormalFrag;
in vec2 UVFrag;

uniform TexturedMaterial uMaterial;
uniform DirectionalLight uDirectionalLight;
uniform PointLight uPointLight;
uniform SpotLight uSpotLight;
uniform vec3 uCameraPos;

layout(location = 0) out vec4 FragColor;
layout(location = 1) out vec4 Normal;

void main()
{
    vec2 uv = uMaterial.DiffuseMapTiling * UVFrag;
    vec3 diffuseMapColor = texture(uMaterial.DiffuseMap, uv).rgb;
    float specularMapColor = texture(uMaterial.SpecularMap, uv).r;

    vec3 normal = normalize(NormalFrag);

    // Ambient 
    vec3 ambientColor = vec3(0.0f);
    // Directinal Light
    ambientColor += CalcAmbient(uDirectionalLight.color, uMaterial.material.ambient);
    // Point Light
    //float fAttenuation = CalcAttenuation(uPointLight.position, PositionFrag);
    //ambientColor += CalcAmbient(uPointLight.color, uMaterial.material.ambient) * fAttenuation;
    // Spot Light
    //fAttenuation = CalcAttenuation(uSpotLight.position, PositionFrag);
    //ambientColor += CalcAmbient(uSpotLight.color, uMaterial.material.ambient) * fAttenuation;
    ambientColor *= diffuseMapColor;

    // Diffuse
    vec3 diffuseColor = vec3(0.0f);
    // Directinal Light
    diffuseColor += Diffuse(uDirectionalLight, normal);
    // Point Light
    diffuseColor += Diffuse(uPointLight, normal, PositionFrag);
     // Spot Light
    diffuseColor += Diffuse(uSpotLight, normal, PositionFrag);
    diffuseColor *= uMaterial.material.diffuse * diffuseMapColor;

    // Specular ( Blin-Phong )
    vec3 specularColor = vec3(0.0f);
    // Directinal Light
    specularColor += Specular(uDirectionalLight, normal, PositionFrag, uCameraPos, uMaterial.material);
    // Point Light
    specularColor += Specular(uPointLight, normal, PositionFrag, uCameraPos, uMaterial.material);
    // Spot Light
    specularColor += Specular(uSpotLight, normal, PositionFrag, uCameraPos, uMaterial.material);
    specularColor *= uMaterial.material.specular * specularMapColor;

    vec3 finalColor = vec3(0.0f);
    finalColor += ambientColor;
    finalColor += diffuseColor;
    finalColor += specularColor;

    vec3 normalColor;
    normalColor.r = remap(normal.r, -1.0, 1.0, 0.0, 1.0);
    normalColor.g = remap(normal.g, -1.0, 1.0, 0.0, 1.0);
    normalColor.b = remap(normal.b, -1.0, 1.0, 0.0, 1.0);
    FragColor = vec4(finalColor, 1.0);
    Normal = vec4(normalColor, 1.0);
};