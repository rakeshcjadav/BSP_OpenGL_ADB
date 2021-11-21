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

struct TexturedMaterial
{
    Material material;
    sampler2D DiffuseMap;
    sampler2D SpecularMap;
};

struct PointLight
{
    // Position of light in world space
    vec3 position;
    vec3 color;
    vec3 attenuation;
};

uniform TexturedMaterial uMaterial;
uniform PointLight uPointLight;
uniform vec3 uCameraPos;

out vec4 FragColor;

void main()
{
    vec3 diffuseMapColor = texture(uMaterial.DiffuseMap, UVFrag).rgb;
    float specularMapColor = texture(uMaterial.SpecularMap, UVFrag).r;

    // Ambient 
    vec3 ambientColor = uPointLight.color * uMaterial.material.ambient * diffuseMapColor;

    // Diffuse
    vec3 lightDirection = normalize(uPointLight.position - PositionFrag);
    float diffuseComponent = max(dot(lightDirection, NormalFrag), 0.0);
    float fDist = distance(uPointLight.position, PositionFrag);
    float fAttenuation = 1.0f/(uPointLight.attenuation.x+uPointLight.attenuation.y*fDist+ uPointLight.attenuation.z*fDist*fDist);
    vec3 diffuseColor = uPointLight.color  * (vec3(diffuseComponent) * uMaterial.material.diffuse) * diffuseMapColor;

    // Specular ( Phong )
    //vec3 cameraDirection = normalize(uCameraPos - PositionFrag);
    //vec3 lightReflection = reflect(-lightDirection, NormalFrag);
    //float specularComponent = pow(max(dot(lightReflection, cameraDirection), 0.0), 2);
    //vec3 specularColor = vec3(specularComponent) * uLightColor;

    // Specular ( Blin-Phong )
    vec3 cameraDirection = normalize(uCameraPos - PositionFrag);
    vec3 halfwayDirection = normalize(cameraDirection + lightDirection);
    float specularComponent = pow(max(dot(halfwayDirection, NormalFrag), 0.0), uMaterial.material.shininess);
    vec3 specularColor = uPointLight.color * (vec3(specularComponent) * uMaterial.material.specular) * specularMapColor;

    vec3 finalColor = ambientColor + diffuseColor + specularColor;

    FragColor = vec4(finalColor*fAttenuation, 1.0);
};