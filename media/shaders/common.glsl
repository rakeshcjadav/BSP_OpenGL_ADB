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

// Viridis colormap - perceptually uniform colormap
vec3 viridis(float t)
{
    t = clamp(t, 0.0, 1.0);
    
    const vec3 c0 = vec3(0.267004, 0.004874, 0.329415);
    const vec3 c1 = vec3(0.127568, 0.566949, 0.550556);
    const vec3 c2 = vec3(0.993248, 0.906157, 0.143936);
    
    float t2 = t * t;
    float t3 = t2 * t;
    float t4 = t3 * t;
    
    vec3 color = vec3(0.0);
    color += c0;
    color += c1 * t;
    color -= c1 * t2 * 0.5;
    color += c2 * t2;
    color -= c2 * t3 * 0.666;
    color += (vec3(0.847, 0.855, 0.788) - c0 - c1 + c1 * 0.5 - c2 + c2 * 0.666) * t3;
    
    return color;
}

// Turbo colormap - improved rainbow colormap
vec3 turbo(float t)
{
    t = clamp(t, 0.0, 1.0);
    
    const vec4 kRedVec4 = vec4(0.13572138, 4.61539260, -42.66032258, 132.13108234);
    const vec4 kGreenVec4 = vec4(0.09140261, 2.19418839, 4.84296658, -14.18503333);
    const vec4 kBlueVec4 = vec4(0.10667330, 12.64194608, -60.58204836, 110.36276771);
    const vec2 kRedVec2 = vec2(-152.94239396, 59.28637943);
    const vec2 kGreenVec2 = vec2(4.27729857, 2.82956604);
    const vec2 kBlueVec2 = vec2(-89.90310912, 27.34824973);
    
    float t2 = t * t;
    float t3 = t2 * t;
    float t4 = t3 * t;
    
    vec3 color;
    color.r = clamp(kRedVec4.x + kRedVec4.y * t + kRedVec4.z * t2 + kRedVec4.w * t3 + kRedVec2.x * t4 + kRedVec2.y * t4 * t, 0.0, 1.0);
    color.g = clamp(kGreenVec4.x + kGreenVec4.y * t + kGreenVec4.z * t2 + kGreenVec4.w * t3 + kGreenVec2.x * t4 + kGreenVec2.y * t4 * t, 0.0, 1.0);
    color.b = clamp(kBlueVec4.x + kBlueVec4.y * t + kBlueVec4.z * t2 + kBlueVec4.w * t3 + kBlueVec2.x * t4 + kBlueVec2.y * t4 * t, 0.0, 1.0);
    
    return color;
}

// Jet colormap - classic rainbow colormap
vec3 jet(float t)
{
    t = clamp(t, 0.0, 1.0);
    
    vec3 color;
    color.r = clamp(1.5 - abs(4.0 * t - 3.0), 0.0, 1.0);
    color.g = clamp(1.5 - abs(4.0 * t - 2.0), 0.0, 1.0);
    color.b = clamp(1.5 - abs(4.0 * t - 1.0), 0.0, 1.0);
    
    return color;
}