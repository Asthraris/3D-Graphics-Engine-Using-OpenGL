#version 450 core
#define MAX_LIGHTS 10
out vec4 OUTPUT;

in vec3 Normal;
in vec3 fragPos;
in float ViewDepth;

struct light {
    int Type;//16 byte

    vec3 Pos;
    float padding1;   // Add padding to match std140 alignment  in std140 vec3 aligns same data as vec4=16bytes hence i added padding of 4bit float next to it to make 16 bit after 12 bit vec3
    //16byte

    vec3 Color;
    float padding2;   // Add padding for the next vec3
    //16byte

    float Intensity; //16  byte
};


uniform int NUM_LIGHTS ;

//make it uniform
vec3 grass = vec3(0.0,0.6,0.0);
vec3 rock = vec3(0.3,0.3,0.3);

vec3 fog = vec3(0.5,0.5,0.5);
float fogDensity = 0.05;

float DARKNESS = 0.3;

layout(std140,binding=0)uniform SETTINGS{
    bool fog_enable ;
	float fog_density ;
	vec3 fog_color ;
	float ambient;
	int num_lights ;
	bool light_enable ;
};

layout(std140,binding=1)uniform LIGHTS{
	light Lights[MAX_LIGHTS];
};

//filhaal ke liye specular light me nhi le raha hu
vec3 CalcLighting(vec3 normal,vec3 fragPos){

	vec3 result = vec3(0.0);

	for(int i =0;i<NUM_LIGHTS;i=i+1){
		
        if (Lights[i].Type == 0) {          // Directional light
            vec3 lightDir = normalize(Lights[i].Pos);
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

vec3 CalcFoginess(vec3 Base,float depth){
    float fogfactor = exp(-fogDensity * depth);
    fogfactor = clamp(fogfactor,0.0,1.0);
    vec3 result = mix(fog,Base,fogfactor);
    return result;
}



void main(){

	vec3 normal = normalize(Normal);

	float base = smoothstep(0.7,1.0,abs(normal.y));
	vec3 final = mix(rock,grass,base);

	if(light_enable==true){
		vec3 litness = CalcLighting(normal,fragPos);
		final = litness * final;
    }

    if(fog_enable==true){
	    final = CalcFoginess(final,ViewDepth);
    }

    OUTPUT = vec4(final,1.0);

}





