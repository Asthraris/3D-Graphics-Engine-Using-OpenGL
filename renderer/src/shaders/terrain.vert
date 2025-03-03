#version 450 core

layout(location=0)in vec3 vPos;
layout(location=1)in vec3 vNormal;


uniform mat4 camMatrix;
uniform mat4 Model_Matrix;

out vec3 Normal;
out vec3 fragPos;


void main(){
	fragPos = vec3( Model_Matrix * vec4( vPos , 1.0 ) );
	gl_Position = camMatrix *Model_Matrix * vec4(vPos,1.0);
	Normal = vNormal;
};
