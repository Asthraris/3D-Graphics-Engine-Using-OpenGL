#version 450 core

layout(location=0)in vec3 vPos;
layout(location=1)in vec3 vNormal;


uniform mat4 camMatrix;
uniform mat4 ModelMatrix;

out vec3 Normal;
out vec3 fragPos;


void main(){
	fragPos = vec3( ModelMatrix * vec4( vPos , 1.0 ) );
	gl_Position = camMatrix * vec4(fragPos,1.0);
	Normal = vec3((transpose(inverse(ModelMatrix))) * vec4(vNormal,1.0));
};
