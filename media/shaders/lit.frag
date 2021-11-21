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

uniform Material uMaterial;

uniform vec3 uLightPos;
uniform vec3 uLightColor;

uniform vec3 uCameraPos;

out vec4 FragColor;

void main()
{
    // Ambient 
    vec3 ambientColor = uLightColor * uMaterial.ambient;

    // Diffuse
    vec3 lightDirection = normalize(uLightPos - PositionFrag);
    float diffuseComponent = max(dot(lightDirection, NormalFrag), 0.0);
    vec3 diffuseColor = uLightColor * (vec3(diffuseComponent) * uMaterial.diffuse);
    
    // Specular ( Phong )
    //vec3 cameraDirection = normalize(uCameraPos - PositionFrag);
    //vec3 lightReflection = reflect(-lightDirection, NormalFrag);
    //float specularComponent = pow(max(dot(lightReflection, cameraDirection), 0.0), 2);
    //vec3 specularColor = vec3(specularComponent) * uLightColor;

    // Specular ( Blin-Phong )
    vec3 cameraDirection = normalize(uCameraPos - PositionFrag);
    vec3 halfwayDirection = normalize(cameraDirection + lightDirection);
    float specularComponent = pow(max(dot(halfwayDirection, NormalFrag), 0.0), uMaterial.shininess);
    vec3 specularColor = uLightColor * (vec3(specularComponent) * uMaterial.specular);

    vec3 finalColor = ambientColor + diffuseColor + specularColor;

    FragColor = vec4(finalColor, 1.0);
};