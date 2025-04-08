#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>


#include <GLFW/glfw3.h>
#include "Renderer/src/Shader.hpp"
/// <summary>
/// this is specific style of camera used in editor softwares for easy usade unilike from free style carema which i was using previously
/// now i will change this later
/// </summary>
namespace eng{
	class ArcBall
	{
	private:
		glm::vec3 UP_VECTOR, FOCUS_VECTOR, POSITION_VECTOR;
		glm::mat4 VIEW_MAT, PERSPECTIVE_MAT;
	
		float FIELD_OF_VIEW, NEAR_PLANE, FAR_PLANE, ASPECT_RATIO;
	
	
	public:
		ArcBall(float fov, float near, float far, float Aspectrat);
		~ArcBall();
		float* renderView();
		glm::vec3 Move(const float deltaTime, GLFWwindow* window);
		void Look(const float deltaTime, GLFWwindow* window);
		float* getProjMatrix();
	
	};
}
