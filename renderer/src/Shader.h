#pragma once

//for camera matrix update
#include <glm/glm.hpp>
class Shader
{
private:
	unsigned int PROGRAM_ID;
	int CAMERA_MAT_LOC;
public:
	Shader(const char* Vertpath,const char* Fragpath);
	~Shader();
	void camMatrix(const glm::mat4& Value);
	void UpdateModelMatrix(const char* name, float* Value);
	unsigned int Activate();
};

