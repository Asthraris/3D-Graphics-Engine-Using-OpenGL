#version 450 core

layout(location=0)in vec3 vPos;
layout(location=1)in vec3 vNormal;


uniform mat4 camMatrix;
uniform mat4 ModelMatrix;

out vec3 Normal;


void main(){
	gl_Position = camMatrix * ModelMatrix * vec4(vPos,1.0);
	Normal = vNormal;
};
