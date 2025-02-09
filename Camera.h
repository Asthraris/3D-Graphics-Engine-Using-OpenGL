#pragma once

#include <glm/glm.hpp>

#include <GLFW/glfw3.h>
#include "renderer/src/Shader.h"

class Camera
{
private:
	glm::vec3 UP_VECTOR, FOCUS_VECTOR, POSITION_VECTOR;
	glm::mat4 VIEW_MAT, PERSPECTIVE_MAT;

	float FIELD_OF_VIEW, NEAR_PLANE, FAR_PLANE, ASPECT_RATIO;

	glm::vec2 CAM_CHUNK;
	
public:
	Camera(float fov, float near,float far, float Aspectrat);
	~Camera();
	void renderView(Shader& active);
	void Move(const float deltaTime, GLFWwindow* window);
	void Look(const float deltaTime, GLFWwindow* window);
	glm::vec2 giveCamChunk();

};

