#include "ArcBall.hpp"




#include <glm/gtc/matrix_transform.hpp>
#include <glm/matrix.hpp>





const float SPEED = 3.0f;
const float SENSITIVITY = 90.0f;
// ye sab config values hai 
bool FIRST_MOVE = true;


eng::ArcBall::ArcBall(float fov, float near, float far, float Aspectrat) :
	FIELD_OF_VIEW(glm::radians(fov)),
	NEAR_PLANE(near),
	FAR_PLANE(far),
	ASPECT_RATIO(Aspectrat)
{
	UP_VECTOR = glm::vec3(0.0f , 1.0f , 0.0f);
	FOCUS_VECTOR = glm::vec3(0.0f,0.0f,3.0f);
	POSITION_VECTOR = glm::vec3(0.0f, 0.0f, 0.0f);

	VIEW_MAT = glm::mat4(1.0f);
	PERSPECTIVE_MAT = glm::perspective(FIELD_OF_VIEW,ASPECT_RATIO,NEAR_PLANE,FAR_PLANE);
}

eng::ArcBall::~ArcBall()
{
	
}

float* eng::ArcBall::renderView(){
	
	VIEW_MAT = glm::mat4(1.0f);
	VIEW_MAT = glm::lookAt(POSITION_VECTOR,POSITION_VECTOR+ FOCUS_VECTOR, UP_VECTOR);
	
	return glm::value_ptr(VIEW_MAT);
}

glm::vec3 eng::ArcBall::Move(const float deltaTime,GLFWwindow* window)
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
	
	return POSITION_VECTOR;
}

void eng::ArcBall::Look(const float deltaTime, GLFWwindow* window) {
	static double lastMousePos_X = 0.0f;
	static double lastMousePos_Y = 0.0f;
	static float YAW = -90.0f;
	static float PITCH = 0.0f;

	double currMousePos_X, currMousePos_Y;
	glfwGetCursorPos(window, &currMousePos_X, &currMousePos_Y);

	// Handle first move to initialize the last mouse position
	if (FIRST_MOVE) {
		lastMousePos_X = currMousePos_X;
		lastMousePos_Y = currMousePos_Y;
		FIRST_MOVE = false;
	}

	// Calculate the mouse offsets
	float offset_X = SENSITIVITY * (float)(currMousePos_X - lastMousePos_X) * deltaTime;
	float offset_Y = SENSITIVITY * (float)(lastMousePos_Y - currMousePos_Y) * deltaTime;
	
	// Update YAW and PITCH
	YAW += offset_X;
	PITCH += offset_Y;
	
	// Clamp PITCH in degrees to avoid gimbal lock
	if (PITCH > 89.0f) PITCH = 89.0f;
	if (PITCH < -89.0f) PITCH = -89.0f;

	// Convert YAW and PITCH to radians for calculations
	float yawRad = glm::radians(YAW);
	float pitchRad = glm::radians(PITCH);
	
	// Calculate the new focus vector
	glm::vec3 tempFocus;
	tempFocus.x = cos(yawRad) * cos(pitchRad);
	tempFocus.y = sin(pitchRad);
	tempFocus.z = sin(yawRad) * cos(pitchRad);
	FOCUS_VECTOR = glm::normalize(tempFocus);
	
	// Update the last mouse position
	lastMousePos_X = currMousePos_X;
	lastMousePos_Y = currMousePos_Y;
}

float* eng::ArcBall::getProjMatrix()
{
	return glm::value_ptr(PERSPECTIVE_MAT);
}



