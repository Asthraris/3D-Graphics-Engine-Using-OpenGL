#version 450 core
out vec4 OUTPUT;

in vec3 Normal;

float Ambient(){
	return 0.95;
}

float Diffuse(vec3 normal,vec3 lightdir){
//yaha res neg bhi aata hai but we want res {0,1} hence max use kiya hai ,it will take max value amoung both value

	float res = max( dot(normal,lightdir), 0.0);
	return res;
}

vec3 grass = vec3(0.0,0.8,0.0);
vec3 rock = vec3(0.3,0.3,0.3);
vec3 env = normalize(vec3(138, 141, 212));
vec3 sunlight = vec3(1.0, 0.7, 0.3);
vec3 sunrays = normalize(vec3(0.3, -0.4, -0.9));

void main(){
		vec3 normal = normalize(Normal);
		float base = smoothstep(0.7,1.0,abs(normal.y));
		vec3 terrainColor = mix(rock,grass,base);
		
		vec3 lightedArea = (Diffuse(normal,sunrays)*2)*sunlight;
		vec3 darkArea = (Ambient()*2)*env;

		

		vec3 final = (darkArea+lightedArea)*terrainColor;
		
		OUTPUT = vec4(final,1.0);
}