#version 460 core
#define MAX_LIGHTS 30
out vec4 OUTPUT;

in vec3 Color;
in vec3 Normal;
in vec3 fragPos;
in float ViewDepth;

struct light {
    int Type;
    vec3 Pos;       float padding1;
    vec3 lightColor; float padding2;
    float Intensity;
};

uniform int NUM_LIGHTS;
uniform vec3 CamPos; // Camera position
uniform float specularStrength = 2; // Strength of specular lighting

layout(std140, binding=1) uniform SETTINGS {
    float ambient;
    int light_enable;
};

layout(std140, binding=0) uniform LIGHTS {
    light Lights[MAX_LIGHTS];
};

vec3 CalcLighting(vec3 normal, vec3 fragPos) {
    vec3 result = vec3(0.0);
    int SHINY_FACTOR = 16;
    vec3 viewDir = normalize(CamPos - fragPos); // View direction

    for (int i = 0; i < NUM_LIGHTS; i++) {
        vec3 lightDir;
        float attenuation = 1.0;

        if (Lights[i].Type == 0) { // Directional light
            lightDir = normalize(-Lights[i].Pos);
        } else { // Point light
            lightDir = normalize(Lights[i].Pos - fragPos);
            float dist = length(Lights[i].Pos - fragPos);
            attenuation = 1.0 / (1.0 + 0.09 * dist + 0.032 * (dist * dist));
        }

        // Diffuse lighting
        float diff = max(dot(normal, lightDir), 0.0);

        // Specular lighting (Blinn-Phong)
        vec3 halfwayDir = normalize(lightDir + viewDir);
        float spec = pow(max(dot(normal, halfwayDir), 0.0), SHINY_FACTOR); // Shininess factor

        vec3 lightEffect = Lights[i].lightColor * Lights[i].Intensity * attenuation;
        result += lightEffect * (diff + specularStrength * spec);
    }
    
    return result + vec3(ambient);
}



void main() {
    vec3 normal = normalize(Normal);
    vec3 final = Color; // Initialize final color

    if (light_enable != 0) {
        vec3 litness = CalcLighting(normal, fragPos);
        final *= litness;
    }


    OUTPUT = vec4(final, 1.0); // Output corrected color
}
