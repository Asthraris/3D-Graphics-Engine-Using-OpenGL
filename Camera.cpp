#include "Camera.h"


#include <glm/gtc/matrix_transform.hpp>
#include <glm/matrix.hpp>
//for upadting matix

const float SPEED = 3.0f;
const float SENSITIVITY = 90.0f;
// ye sab config values hai 
bool FIRST_MOVE = true;


Camera::Camera(float fov, float near, float far, float Aspectrat) :
	FIELD_OF_VIEW(glm::radians(fov)),
	NEAR_PLANE(near),
	FAR_PLANE(far),
	ASPECT_RATIO(Aspectrat)
{
	UP_VECTOR = glm::vec3(0.0f , 1.0f , 0.0f);
	FOCUS_VECTOR = glm::vec3(0.0f,0.0f,0.0f);
	POSITION_VECTOR = glm::vec3(0.0f, 0.0f, 3.0f);

	VIEW_MAT = glm::mat4(1.0f);
	PERSPECTIVE_MAT = glm::perspective(FIELD_OF_VIEW,ASPECT_RATIO,NEAR_PLANE,FAR_PLANE);
}

Camera::~Camera()
{

}

void Camera::renderView(Shader& active){
	
	VIEW_MAT = glm::mat4(1.0f);
	VIEW_MAT = glm::lookAt(POSITION_VECTOR,POSITION_VECTOR+ FOCUS_VECTOR, UP_VECTOR);
	VIEW_MAT = PERSPECTIVE_MAT * VIEW_MAT;
	active.camMatrix(VIEW_MAT);
	
}

void Camera::Move(const float deltaTime,GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		POSITION_VECTOR += SPEED * FOCUS_VECTOR * deltaTime;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		POSITION_VECTOR += SPEED * (-FOCUS_VECTOR) * deltaTime;

	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		POSITION_VECTOR += SPEED * glm::normalize(glm::cross(UP_VECTOR, FOCUS_VECTOR)) * deltaTime;

	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		POSITION_VECTOR += SPEED * glm::normalize(glm::cross(FOCUS_VECTOR, UP_VECTOR)) * deltaTime;

	}
	if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS) {
		POSITION_VECTOR += SPEED * UP_VECTOR * deltaTime;

	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
		POSITION_VECTOR += SPEED * (-UP_VECTOR) * deltaTime;

	}
}

void Camera::Look(const float deltaTime, GLFWwindow* window) 
	{
		static double lastXpos, lastYpos;
		double Xpos, Ypos;
		float offsetX, offsetY;
		//mistake 1 - not making yaw nas pitch static
		static float yaw = -90.0f, pitch = 0.0f;
		glm::vec3 tempFocus;

		glfwGetCursorPos(window, &Xpos, &Ypos);
		if (FIRST_MOVE) {
			lastXpos = Xpos;
			lastYpos = Ypos;
			FIRST_MOVE = false;
		}
		offsetX = SENSITIVITY * (float)(Xpos - lastXpos) * deltaTime;
		offsetY = SENSITIVITY * (float)(lastYpos - Ypos) * deltaTime;// idk why its reversed do research

		/*Yaw and Pitch are two of the three angles used to describe the orientation of an object in a 3D space.The third angle is Roll.

			Yaw: This is the rotation around the vertical axis(usually the y - axis).Think of yaw as turning your head left or right.If you imagine looking down at an object from above, yaw would be the angle you turn left or right.

			Pitch : This is the rotation around the lateral or horizontal axis(usually the x - axis).Imagine nodding your head up and down; that's pitch. It tilts the object up or down.*/

			// yaw & pitch records czmera angle preview and used to set next value
		yaw += offsetX;
		pitch += offsetY;

		if (pitch > 89.0f) pitch = 89.0f;
		if (pitch < -89.0f) pitch = -89.0f;
		/*
		agar y me topmost ans bottom most lock karna hotoh
		*/

		tempFocus.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		tempFocus.y = sin(glm::radians(pitch));
		tempFocus.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		FOCUS_VECTOR = glm::normalize(tempFocus);
		lastXpos = Xpos;
		lastYpos = Ypos;
	}

