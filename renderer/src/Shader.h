#pragma once

//for camera matrix update
#include <glm/glm.hpp>
class Shader
{
private:
	unsigned int PROGRAM_ID;
	int CAMERA_MAT_LOC;
	int MODEL_MAT_LOC;
public:
	Shader();
	Shader(const char* Vertpath,const char* Fragpath);
	~Shader();
	void camMatrix(const float* Value);
	void UpdateModelMatrix(const float* Value);
	void Activate();
	unsigned int getProgram();
};

