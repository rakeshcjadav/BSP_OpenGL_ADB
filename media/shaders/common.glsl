//? #version 330 core

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

float remap(float s, float a1, float a2, float b1, float b2)
{
    return b1 + (s - a1) * (b2 - b1) / (a2 - a1);
}

vec3 CalcAmbient(vec3 lightColor, vec3 objectColor)
{
    return lightColor * objectColor;
}

float CalcDiffuse(vec3 lightDirection, vec3 normal)
{
    return max(dot(lightDirection, normal), 0.0);
}

float CalcSpecular(vec3 cameraPos, vec3 lightDirection, vec3 fragPos, vec3 fragNormal, Material mat)
{
    vec3 lightDir = normalize(lightDirection);
    vec3 cameraDirection = normalize(cameraPos - fragPos);
    vec3 halfwayDirection = normalize(cameraDirection + lightDir);
    return pow(max(dot(halfwayDirection, fragNormal), 0.0), mat.shininess) * mat.specularStrength;
}

float CalcAttenuation(vec3 lightPos, vec3 atten, vec3 fragPos)
{
    float fDist = distance(lightPos, fragPos);
    return 1.0f / (atten.x + atten.y * fDist + atten.z * fDist * fDist);
}

vec3 Diffuse(DirectionalLight directionalLight, vec3 normal)
{
    float diffuseComponent = CalcDiffuse(directionalLight.direction, normal);
    vec3 diffuseColor = diffuseComponent * directionalLight.color;
    return diffuseColor;
}

vec3 Diffuse(PointLight pointLight, vec3 normal, vec3 pos)
{
    vec3 lightDirection = normalize(pointLight.position - pos);
    float diffuseComponent = CalcDiffuse(lightDirection, normal);
    float fAttenuation = CalcAttenuation(pointLight.position, pointLight.attenuation, pos);
    vec3 diffuseColor = diffuseComponent * pointLight.color * fAttenuation;
    return diffuseColor;
}

vec3 Diffuse(SpotLight spotLight, vec3 normal, vec3 pos)
{
    vec3 lightDirection = normalize(spotLight.position - pos);
    float cosTheta = max(dot(lightDirection, -spotLight.direction), 0.0f);
    float cosInnerCutOff = cos(spotLight.cutoffAngle);
    float cosOuterCutOff = cos(spotLight.outerCutOffAngle);
    float diffuseComponent = 0.0f;
    vec3 diffuseColor;
    if (cosTheta > cosOuterCutOff)
    {
        float epsilon = cosInnerCutOff - cosOuterCutOff;
        diffuseComponent = clamp((cosTheta - cosOuterCutOff) / epsilon, 0.0f, 1.0f) * CalcDiffuse(lightDirection, normal);
        //diffuseComponent = remap(cosTheta, cosInnerCutOff, 1.0f, 0.0f, 1.0f);
        float fAttenuation = CalcAttenuation(spotLight.position, spotLight.attenuation, pos);
        diffuseComponent *= fAttenuation;
    }

    return diffuseColor = diffuseComponent * spotLight.color;
}

vec3 Specular(DirectionalLight directionalLight, vec3 normal, vec3 pos, vec3 cameraPos, Material mat)
{
    return directionalLight.color * CalcSpecular(cameraPos, directionalLight.direction, pos, normal, mat);
}

vec3 Specular(PointLight pointLight, vec3 normal, vec3 pos, vec3 cameraPos, Material mat)
{
    vec3 lightDirection = normalize(pointLight.position - pos);
    float fAttenuation = CalcAttenuation(pointLight.position, pointLight.attenuation, pos);
    return pointLight.color * CalcSpecular(cameraPos, lightDirection, pos, normal, mat) * fAttenuation;
}

vec3 Specular(SpotLight spotLight, vec3 normal, vec3 pos, vec3 cameraPos, Material mat)
{
    vec3 lightDirection = normalize(spotLight.position - pos);
    float fAttenuation = CalcAttenuation(spotLight.position, spotLight.attenuation, pos);
    return spotLight.color * CalcSpecular(cameraPos, lightDirection, pos, normal, mat) * fAttenuation;
}

// TODO: pass as uniform
float near = 0.1f;
float far = 100.0f;

float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0; // back to NDC 
    return (2.0 * near * far) / (far + near - z * (far - near));
}