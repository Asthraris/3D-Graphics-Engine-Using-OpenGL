#version 450 core
out vec4 OUTPUT;

in vec3 Color;
in float Height;

void main(){
	
		OUTPUT = vec4(Color,1.0);

}