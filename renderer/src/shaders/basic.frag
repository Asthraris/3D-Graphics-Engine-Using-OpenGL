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

layout(std140, binding=1) uniform SETTINGS {
    float ambient;
    int light_enable;
    int fog_enable;
    float fog_density;
    vec3 fog_color;
};

layout(std140, binding=0) uniform LIGHTS {
    light Lights[MAX_LIGHTS];
};

vec3 CalcLighting(vec3 normal, vec3 fragPos) {
    vec3 result = vec3(0.0);
    for (int i = 0; i < NUM_LIGHTS; i++) {
        if (Lights[i].Type == 0) { // Directional light
            vec3 lightDir = normalize(-Lights[i].Pos);
            float diff = max(dot(normal, lightDir), 0.0);
            result += Lights[i].lightColor * Lights[i].Intensity * diff;
        } else { // Point light
            vec3 lightDir = normalize(Lights[i].Pos - fragPos);
            float diff = max(dot(normal, lightDir), 0.0);
            float dist = length(Lights[i].Pos - fragPos);
            float attenuation = 1.0 / (1.0 + 0.09 * dist + 0.032 * (dist * dist));
            result += Lights[i].lightColor * Lights[i].Intensity * diff * attenuation;
        }
    }
    return result + vec3(ambient);
}

float CalcFoginess(float depth) {
    return clamp(exp(-fog_density * depth), 0.0, 1.0);
}

void main() {
    vec3 normal = normalize(Normal);
    vec3 final = Color; // Initialize final color

    if (light_enable != 0) {
        vec3 litness = CalcLighting(normal, fragPos);
        final *= litness;
    }

    if (fog_enable != 0) {
        final = mix(fog_color, final, CalcFoginess(ViewDepth));
    }

    OUTPUT = vec4(final, 1.0); // Output corrected color
}
