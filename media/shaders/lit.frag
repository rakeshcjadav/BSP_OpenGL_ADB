#version 330 core
#include "common.glsl"

in OUT
{
    // Fragment Position in World Space
    vec3 PositionFrag;
    // Normal Direction of a fragment in World Space
    vec3 NormalFrag;
    vec2 UVFrag;
    flat vec2 ColorRangeFrag;
    vec3 BarycentricFrag;
}fragIn;

uniform Material uMaterial;
uniform DirectionalLight uDirectionalLight;
uniform PointLight uPointLight;
uniform SpotLight uSpotLight;
uniform vec3 uCameraPos;
uniform int uColormapMode;
uniform float uScalarRangeMin;
uniform float uScalarRangeMax;
uniform bool uShowWireframe;
uniform float uWireframeThickness;

layout(location = 0) out vec4 FragColor;

vec3 applyColormap(float t, int mode)
{
    if (mode == 0)
        return viridis(t);
    else if (mode == 1)
        return turbo(t);
    else if (mode == 2)
        return jet(t);
    else
        return viridis(t);
}

float edgeFactor()
{
    vec3 d = fwidth(fragIn.BarycentricFrag);
    vec3 a3 = smoothstep(vec3(0.0), d * uWireframeThickness, fragIn.BarycentricFrag);
    return min(min(a3.x, a3.y), a3.z);
}

void main()
{
    float scalarValue = fragIn.ColorRangeFrag.x;
    
    float minVal = uScalarRangeMin * 2.4 - 1.2;
    float maxVal = uScalarRangeMax * 2.4 - 1.2;
    
    vec3 colormapColor;
    if (scalarValue < minVal)
    {
        discard;
    }
    else if (scalarValue > maxVal)
    {
         discard;
    }
    else
    {
        float normalizedValue = (scalarValue - minVal) / (maxVal - minVal);
        colormapColor = applyColormap(normalizedValue, uColormapMode);
    }
    
    vec3 ambientColor = CalcAmbient(uDirectionalLight.color, uMaterial.ambient);

    vec3 normal = normalize(fragIn.NormalFrag);

    vec3 diffuseColor = vec3(0.0f);
    diffuseColor += Diffuse(uDirectionalLight, normal);

    vec3 finalColor = vec3(0.0f);
    finalColor += ambientColor;
    finalColor += diffuseColor;

    finalColor *= colormapColor;
    
    if (uShowWireframe)
    {
        vec3 wireframeColor = vec3(0.0, 0.0, 0.0);
        float edge = edgeFactor();
        finalColor = mix(wireframeColor, finalColor, edge);
    }

    FragColor = vec4(finalColor, 1.0);
};