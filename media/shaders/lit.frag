#version 330 core
#include "common.glsl"

// Fragment Position in World Space
in vec3 PositionFrag;
// Normal Direction of a fragment in World Space
in vec3 NormalFrag;
in vec2 UVFrag;

uniform Material uMaterial;
uniform DirectionalLight uDirectionalLight;
uniform PointLight uPointLight;
uniform SpotLight uSpotLight;
uniform vec3 uCameraPos;

out vec4 FragColor;


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

    // Diffuse
    vec3 diffuseColor = vec3(0.0f);
    // Directinal Light
    diffuseColor += Diffuse(uDirectionalLight, NormalFrag);
    // Point Light
    diffuseColor += Diffuse(uPointLight, NormalFrag, PositionFrag);
    // Spot Light
    diffuseColor += Diffuse(uSpotLight, NormalFrag, PositionFrag);

    diffuseColor *= uMaterial.diffuse;

    // Specular ( Blin-Phong )
    // Directinal Light
    vec3 specularColor = Specular(uDirectionalLight, NormalFrag, PositionFrag, uCameraPos, uMaterial);
    // Point Light
    specularColor += Specular(uPointLight, NormalFrag, PositionFrag, uCameraPos, uMaterial);
    // Spot Light
    specularColor += Specular(uSpotLight, NormalFrag, PositionFrag, uCameraPos, uMaterial);

    specularColor *= uMaterial.specular;

    vec3 finalColor = vec3(0.0f);
    finalColor += ambientColor;
    finalColor += diffuseColor;
    finalColor += specularColor;

    FragColor = vec4(finalColor, 1.0);

    FragColor = vec4(vec3(gl_FragCoord.z), 1.0);
};