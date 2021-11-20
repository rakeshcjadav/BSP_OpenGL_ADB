#version 330 core

// Fragment Position in World Space
in vec3 PositionFrag;
// Normal Direction of a fragment in World Space
in vec3 NormalFrag;
in vec2 UVFrag;

uniform vec3 uObjectColor;

uniform vec3 uLightPos;
uniform vec3 uLightColor;

uniform vec3 uCameraPos;

out vec4 FragColor;

void main()
{
    // Ambient 
    vec3 ambientColor = uLightColor * vec3(0.1f);

    // Diffuse
    vec3 lightDirection = normalize(uLightPos - PositionFrag);
    float diffuseComponent = max(dot(lightDirection, NormalFrag), 0.0);
    vec3 diffuseColor = vec3(diffuseComponent) * uLightColor;
    
    // Specular ( Phong )
    vec3 cameraDirection = normalize(uCameraPos - PositionFrag);
    vec3 lightReflection = reflect(-lightDirection, NormalFrag);
    float specularComponent = pow(max(dot(lightReflection, cameraDirection), 0.0), 128);
    vec3 specularColor = vec3(specularComponent) * uLightColor;

    // Specular ( Blin-Phong )


    vec3 finalColor = ambientColor + diffuseColor + specularColor;

    FragColor = vec4(uObjectColor, 1.0) * vec4(finalColor, 1.0);
};