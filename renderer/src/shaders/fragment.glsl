#version 450 core
in vec3 Color;

out vec4 OUTPUT;
void main(){

	OUTPUT = vec4(Color,1.0);

}