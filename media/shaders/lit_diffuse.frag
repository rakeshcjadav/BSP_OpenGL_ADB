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
    float specularStrength;
};

struct TexturedMaterial
{
    Material material;
    sampler2D DiffuseMap;
    vec2 DiffuseMapTiling;
    sampler2D SpecularMap;
};

struct DirectionalLight
{
    vec3 direction;
    vec3 color;
};

struct PointLight
{
    // Position of light in world space
    vec3 position;
    vec3 color;
    vec3 attenuation;
};

struct SpotLight
{
    vec3 position;
    vec3 direction;
    vec3 color;
    vec3 attenuation;
    float cutoffAngle;
    float outerCutOffAngle;
};

uniform TexturedMaterial uMaterial;
uniform DirectionalLight uDirectionalLight;
uniform PointLight uPointLight;
uniform SpotLight uSpotLight;
uniform vec3 uCameraPos;

out vec4 FragColor;

vec3 CalcAmbient(vec3 lightColor, vec3 objectColor)
{
    return lightColor * objectColor;
}

float CalcDiffuse(vec3 lightDirection, vec3 normal)
{
    return max(dot(lightDirection, normal), 0.0);
}

float CalcSpecular(vec3 cameraPos, vec3 lightDirection, vec3 fragPos, vec3 fragNormal)
{
    vec3 lightDir = normalize(lightDirection);
    vec3 cameraDirection = normalize(cameraPos - fragPos);
    vec3 halfwayDirection = normalize(cameraDirection + lightDir);
    return pow(max(dot(halfwayDirection, fragNormal), 0.0), uMaterial.material.shininess) * uMaterial.material.specularStrength;
}

float CalcAttenuation(vec3 lightPos, vec3 fragPos)
{
    float fDist = distance(lightPos, fragPos);
    return 1.0f/(uPointLight.attenuation.x+uPointLight.attenuation.y*fDist+ uPointLight.attenuation.z*fDist*fDist);
}

vec3 Diffuse(DirectionalLight directionalLight, vec3 normal)
{
    float diffuseComponent = CalcDiffuse(directionalLight.direction, normal);
    vec3 diffuseColor = diffuseComponent * directionalLight.color;
    return diffuseColor;
}

vec3 Diffuse(PointLight pointLight, vec3 normal)
{
    vec3 lightDirection = normalize(pointLight.position - PositionFrag);
    float diffuseComponent = CalcDiffuse(lightDirection, normal);
    float fAttenuation = CalcAttenuation(pointLight.position, PositionFrag);
    vec3 diffuseColor = diffuseComponent * pointLight.color * fAttenuation;
    return diffuseColor;
}

vec3 Diffuse(SpotLight spotLight, vec3 normal)
{
    vec3 lightDirection = normalize(spotLight.position - PositionFrag);
    float cosTheta = max(dot(lightDirection, -spotLight.direction), 0.0f);
    float cosInnerCutOff = cos(spotLight.cutoffAngle);
    float cosOuterCutOff = cos(spotLight.outerCutOffAngle);
    float diffuseComponent = 0.0f;
    vec3 diffuseColor;
    if(cosTheta > cosOuterCutOff)
    {
        float epsilon = cosInnerCutOff - cosOuterCutOff;
        diffuseComponent = clamp((cosTheta - cosOuterCutOff) / epsilon, 0.0f, 1.0f) * CalcDiffuse(lightDirection, normal);
        //diffuseComponent = remap(cosTheta, cosInnerCutOff, 1.0f, 0.0f, 1.0f);
        float fAttenuation = CalcAttenuation(spotLight.position, PositionFrag);
        diffuseComponent *= fAttenuation;
    }

    return diffuseColor = diffuseComponent * spotLight.color;
}

vec3 Specular(DirectionalLight directionalLight, vec3 normal)
{
    return directionalLight.color * CalcSpecular(uCameraPos, directionalLight.direction, PositionFrag, normal);
}

vec3 Specular(PointLight pointLight, vec3 normal)
{
    vec3 lightDirection = normalize(pointLight.position - PositionFrag);
    float fAttenuation = CalcAttenuation(pointLight.position, PositionFrag);
    return pointLight.color * CalcSpecular(uCameraPos, lightDirection, PositionFrag, normal) * fAttenuation;
}

vec3 Specular(SpotLight spotLight, vec3 normal)
{
    vec3 lightDirection = normalize(spotLight.position - PositionFrag);
    float fAttenuation = CalcAttenuation(spotLight.position, PositionFrag);
    return spotLight.color * CalcSpecular(uCameraPos, lightDirection, PositionFrag, normal) * fAttenuation;
}


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
    diffuseColor += Diffuse(uPointLight, normal);
     // Spot Light
    diffuseColor += Diffuse(uSpotLight, normal);
    diffuseColor *= uMaterial.material.diffuse * diffuseMapColor;

    // Specular ( Blin-Phong )
    vec3 specularColor = vec3(0.0f);
    // Directinal Light
    specularColor += Specular(uDirectionalLight, normal);
    // Point Light
    specularColor += Specular(uPointLight, normal);
    // Spot Light
    specularColor += Specular(uSpotLight, normal);
    specularColor *= uMaterial.material.specular * specularMapColor;

    vec3 finalColor = vec3(0.0f);
    finalColor += ambientColor;
    finalColor += diffuseColor;
    finalColor += specularColor;

    FragColor = vec4(finalColor, 1.0);
};