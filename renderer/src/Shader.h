#pragma once

//for camera matrix update
#include <glm/glm.hpp>
class Shader
{
private:
	unsigned int PROGRAM_ID;
	int VIEW_MAT_LOC;
	int PROJ_MAT_LOC;
	int NUM_LIGHTS_LOC;
	int CAM_POS_LOC;
	unsigned int LIGHT_BLOCK_LOC;
	unsigned int CONFIG_BLOCK_LOC;

public:
	Shader();
	Shader(const char* Vertpath,const char* Fragpath);
	~Shader();
	void projMatrix(const float* Value);
	void UpdateCamPos( glm::vec3 Value);
	void viewMatrix(const float* Value);
	void UpdateNUM_LIGHTS(const int num_lights);
	void Activate();
	unsigned int getProgram();
};

