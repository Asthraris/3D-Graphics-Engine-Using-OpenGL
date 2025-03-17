#version 450 core

layout(location=0)in vec3 vPos;
layout(location=1)in vec3 vNormal;


uniform mat4 View_Matrix;
uniform mat4 Proj_Matrix;
uniform mat4 Model_Matrix;

out vec3 Normal;
out vec3 fragPos;
out float ViewDepth;


void main(){
	vec4 World_pos = Model_Matrix * vec4( vPos , 1.0 ) ;
	vec4 View_pos = View_Matrix * World_pos;

	gl_Position = Proj_Matrix * View_pos;

	ViewDepth = length( View_pos.xyz );
	fragPos = World_pos.xyz;
	Normal = vNormal;
};
