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

uniform Material uMaterial;
uniform DirectionalLight uDirectionalLight;
uniform PointLight uPointLight;
uniform SpotLight uSpotLight;
uniform vec3 uCameraPos;

out vec4 FragColor;

float remap(float s, float a1, float a2, float b1, float b2)
{
    return b1 + (s-a1)*(b2-b1)/(a2-a1);
}

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
    vec3 cameraDirection = normalize(cameraPos - fragPos);
    vec3 halfwayDirection = normalize(cameraDirection + lightDirection);
    return pow(max(dot(halfwayDirection, fragNormal), 0.0), uMaterial.shininess) * uMaterial.specularStrength;
}

float CalcAttenuation(vec3 lightPos, vec3 fragPos)
{
    float fDist = distance(lightPos, fragPos);
    return 1.0f/(uPointLight.attenuation.x+uPointLight.attenuation.y*fDist+ uPointLight.attenuation.z*fDist*fDist);
}

vec3 Diffuse(DirectionalLight directionalLight)
{
    float diffuseComponent = CalcDiffuse(directionalLight.direction, NormalFrag);
    vec3 diffuseColor = diffuseComponent * directionalLight.color;
    return diffuseColor;
}

vec3 Diffuse(PointLight pointLight)
{
    vec3 lightDirection = normalize(pointLight.position - PositionFrag);
    float diffuseComponent = CalcDiffuse(lightDirection, NormalFrag);
    float fAttenuation = CalcAttenuation(pointLight.position, PositionFrag);
    vec3 diffuseColor = diffuseComponent * pointLight.color * fAttenuation;
    return diffuseColor;
}

vec3 Diffuse(SpotLight spotLight)
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
        diffuseComponent = clamp((cosTheta - cosOuterCutOff) / epsilon, 0.0f, 1.0f);
        //diffuseComponent = remap(cosTheta, cosInnerCutOff, 1.0f, 0.0f, 1.0f);
        float fAttenuation = CalcAttenuation(spotLight.position, PositionFrag);
        diffuseComponent *= fAttenuation;
    }

    return diffuseColor = diffuseComponent * spotLight.color;
}

vec3 Specular(DirectionalLight directionalLight)
{
    return directionalLight.color * CalcSpecular(uCameraPos, directionalLight.direction, PositionFrag, NormalFrag);
}

vec3 Specular(PointLight pointLight)
{
    vec3 lightDirection = normalize(pointLight.position - PositionFrag);
    float fAttenuation = CalcAttenuation(pointLight.position, PositionFrag);
    return pointLight.color * CalcSpecular(uCameraPos, lightDirection, PositionFrag, NormalFrag) * fAttenuation;
}

vec3 Specular(SpotLight spotLight)
{
    vec3 lightDirection = normalize(spotLight.position - PositionFrag);
    float fAttenuation = CalcAttenuation(spotLight.position, PositionFrag);
    return spotLight.color * CalcSpecular(uCameraPos, lightDirection, PositionFrag, NormalFrag) * fAttenuation;
}


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
    diffuseColor += Diffuse(uDirectionalLight);
    // Point Light
    diffuseColor += Diffuse(uPointLight);
    // Spot Light
    diffuseColor += Diffuse(uSpotLight);

    diffuseColor *= uMaterial.diffuse;

    // Specular ( Blin-Phong )
    // Directinal Light
    vec3 specularColor = Specular(uDirectionalLight);
    // Point Light
    specularColor += Specular(uPointLight);
    // Spot Light
    specularColor += Specular(uSpotLight);

    specularColor *= uMaterial.specular;

    vec3 finalColor = vec3(0.0f);
    finalColor += ambientColor;
    finalColor += diffuseColor;
    finalColor += specularColor;

    FragColor = vec4(finalColor, 1.0);
};