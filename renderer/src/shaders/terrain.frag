#version 450 core
#define MAX_LIGHTS 30
out vec4 OUTPUT;

in vec3 Normal;
in vec3 fragPos;
in float ViewDepth;

struct light {
    int Type;                // 4 bytes (int)
    vec3 Pos;                 // 12 bytes (padded to 16 bytes)
    float padding1;           // Padding for std140
    vec3 Color;               // 12 bytes (padded to 16 bytes)
    float padding2;           // Padding for std140
    float Intensity;          // 4 bytes (aligned to 16 bytes)
};

uniform int NUM_LIGHTS;

vec3 grass = vec3(0.0, 0.6, 0.0);
vec3 rock = vec3(0.3, 0.3, 0.3);

layout(std140, binding=1) uniform SETTINGS {
    float ambient;            // Ambient lighting factor
    int light_enable;         // Use int (4 bytes, properly aligned)
    int fog_enable;           // Use int (4 bytes)
    float fog_density;        // Fog density
    vec3 fog_color;           // Fog color (std140 padding makes this 16 bytes)
};

layout(std140, binding=0) uniform LIGHTS {
    light Lights[MAX_LIGHTS];
};

//  Lighting Calculation with Ambient Applied
vec3 CalcLighting(vec3 normal, vec3 fragPos) {
    vec3 result = vec3(0.0);

    for (int i = 0; i < NUM_LIGHTS; i++) {
        if (Lights[i].Type == 0) {          // Directional light
            vec3 lightDir = normalize(Lights[i].Pos);
            float diff = max(dot(normal, lightDir), 0.0);
            result += Lights[i].Color * Lights[i].Intensity * diff;

        } else {                            // Point light
            vec3 lightDir = normalize(Lights[i].Pos - fragPos);
            float diff = max(dot(normal, lightDir), 0.0);
            float dist = length(Lights[i].Pos - fragPos);
            float attenuation = 1.0 / (1.0 + 0.09 * dist + 0.032 * (dist * dist));
            result += Lights[i].Color * Lights[i].Intensity * diff * attenuation;
        }
    }

    // Apply ambient lighting
    result += vec3(ambient);   // Add ambient contribution
    return result;
}

float CalcFoginess(float depth) {
    float fogfactor = exp(-fog_density * depth);
    fogfactor = clamp(fogfactor, 0.0, 1.0);
    return fogfactor;
}

void main() {
    vec3 normal = normalize(Normal);

    float base = smoothstep(0.7, 1.0, abs(normal.y));
    vec3 final = mix(rock, grass, base);

    if (light_enable != 0) {
        vec3 litness = CalcLighting(normal, fragPos);
        final = litness * final;   // Apply lighting
    }

    if (fog_enable != 0) {
        final = mix(fog_color, final, CalcFoginess(ViewDepth));  // Apply fog
    }

    OUTPUT = vec4(final, 1.0);
}
