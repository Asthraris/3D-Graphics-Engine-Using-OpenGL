#version 450 core
#define MAX_LIGHTS 10
out vec4 OUTPUT;

in vec3 Normal;
in vec3 fragPos;

struct light{
	int Type;
	vec3 Pos;
	vec3 Color;
	float Intensity;
};

uniform int NUM_LIGHTS;

layout(std140,binding=1)uniform LIGHTS{
	light Lights[MAX_LIGHTS];
};

//filhaal ke liye specular light me nhi le raha hu
vec3 CalcLighting(vec3 normal,vec3 fragPos){

	vec3 result = vec3(0.0);

	for(int i =0;i<NUM_LIGHTS;i++){
		
        if (Lights[i].Type == 1) {          // Directional light
            vec3 lightDir = normalize(-Lights[i].Pos);
            float diff = max(dot(normal, lightDir), 0.0);
            result += Lights[i].Color * Lights[i].Intensity * diff;

        } else {                           // Point light
            vec3 lightDir = normalize(Lights[i].Pos - fragPos);
            float diff = max(dot(normal, lightDir), 0.0);
            float dist = length(Lights[i].Pos - fragPos);
            float attenuation = 1.0 / (1.0 + 0.09 * dist + 0.032 * (dist * dist));
            result += Lights[i].Color * Lights[i].Intensity * diff * attenuation;
        }
    }
	return result;
}

vec3 grass = vec3(0.0,0.6,0.0);
vec3 rock = vec3(0.3,0.3,0.3);

void main(){
		vec3 normal = normalize(Normal);
		float base = smoothstep(0.7,1.0,abs(normal.y));
		vec3 terrainColor = mix(rock,grass,base);
		
		//vec3 litness = CalcLighting(normal,fragPos);

		//vec3 final = terrainColor;
		
		OUTPUT = vec4(terrainColor, 1.0);
}





