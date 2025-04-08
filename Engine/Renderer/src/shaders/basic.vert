#version 460 core

layout(location=0)in vec3 vPos;
layout(location=1)in vec3 vColor;
layout(location=2)in vec3 vNormal;
layout(location=3)in vec2 vTexCoord;

out vec3 Color;
out vec3 Normal;
out vec3 fragPos;
out float ViewDepth;

uniform mat4 View_Matrix;
uniform mat4 Proj_Matrix;

layout(std430,binding =0) buffer Instances{
	mat4 vModels[];
};

void main(){
	//used to find index base instnace gives base of instnace and instance id
	int inst_index = gl_InstanceID + gl_BaseInstance;

	vec4 World_pos = vModels[inst_index] * vec4( vPos , 1.0 ) ;
	vec4 View_pos = View_Matrix * World_pos;

	gl_Position = Proj_Matrix * View_pos;

	ViewDepth = length( View_pos.xyz );
	fragPos = World_pos.xyz;
	Normal = vNormal;
	Color = vColor;
}