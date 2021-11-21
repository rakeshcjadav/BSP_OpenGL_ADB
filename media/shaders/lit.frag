#version 330 core

// Fragment Position in World Space
in vec3 PositionFrag;
// Normal Direction of a fragment in World Space
in vec3 NormalFrag;
in vec2 UVFrag;

struct Material
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct PointLight
{
    // Position of light in world space
    vec3 position;
    vec3 color;
    vec3 attenuation;
};

uniform Material uMaterial;
uniform PointLight uPointLight;
uniform vec3 uCameraPos;

out vec4 FragColor;

void main()
{
    // Ambient 
    vec3 ambientColor = uPointLight.color * uMaterial.ambient;

    // Diffuse
    vec3 lightDirection = normalize(uPointLight.position - PositionFrag);
    float diffuseComponent = max(dot(lightDirection, NormalFrag), 0.0);
    float fDist = distance(uPointLight.position, PositionFrag);
    float fAttenuation = 1.0f/(uPointLight.attenuation.x+uPointLight.attenuation.y*fDist+ uPointLight.attenuation.z*fDist*fDist);
    vec3 diffuseColor = uPointLight.color * (vec3(diffuseComponent) * uMaterial.diffuse);
    
    // Specular ( Phong )
    //vec3 cameraDirection = normalize(uCameraPos - PositionFrag);
    //vec3 lightReflection = reflect(-lightDirection, NormalFrag);
    //float specularComponent = pow(max(dot(lightReflection, cameraDirection), 0.0), 128);
    //vec3 specularColor = vec3(specularComponent) * uLightColor * uMaterial.specular;

    // Specular ( Blin-Phong )
    vec3 cameraDirection = normalize(uCameraPos - PositionFrag);
    vec3 halfwayDirection = normalize(cameraDirection + lightDirection);
    float specularComponent = pow(max(dot(halfwayDirection, NormalFrag), 0.0), uMaterial.shininess);
    vec3 specularColor = uPointLight.color * (vec3(specularComponent) * uMaterial.specular);

    vec3 finalColor = ambientColor + diffuseColor + specularColor;

    FragColor = vec4(finalColor*fAttenuation, 1.0);
};