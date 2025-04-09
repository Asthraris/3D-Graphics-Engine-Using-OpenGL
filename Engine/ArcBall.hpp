#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <algorithm>
#include <iostream>
#include <GLFW/glfw3.h>

#include "Renderer/src/Shader.hpp"

namespace eng {
	class ArcBall {
	private:
		glm::vec3 TARGET_VECTOR, POSITION_VECTOR, UP_VECTOR;
		glm::mat4 PERSPECTIVE_MAT, VIEW_MAT;

		float YAW, PITCH;
		float DIST_FROM_TARGET;

		float FIELD_OF_VIEW;
		float a_sensitivity = 0.3f;
		float a_pan_speed = 0.4f;
		float a_scroll_sensitivity = 0.09f;
		

		void ProcMouseOrbit(float dx, float dy) {
			YAW += dx * a_sensitivity;
			PITCH = glm::clamp(PITCH - dy * a_sensitivity, -89.0f, 89.0f);
			RecalcPos();
		}

		void ProcMousePan(float dx, float dy) {
			glm::vec3 forward = glm::normalize(TARGET_VECTOR - POSITION_VECTOR);
			glm::vec3 right = glm::normalize(glm::cross(forward, UP_VECTOR));
			glm::vec3 up = glm::normalize(glm::cross(right, forward));

			float panScale = DIST_FROM_TARGET * 0.002f;
			glm::vec3 panOffset = (-right * dx + up * dy) * panScale;

			// Move both the camera and the target
			POSITION_VECTOR += panOffset;
			TARGET_VECTOR += panOffset;
		}








		void ProcMouseScroll(float dy) {
			DIST_FROM_TARGET = std::max(0.1f, DIST_FROM_TARGET - dy);
			RecalcPos();
		}

		void RecalcPos() {
			float radYaw = glm::radians(YAW);
			float radPitch = glm::radians(PITCH);
			glm::vec3 direction = {
				cos(radPitch) * cos(radYaw),
				sin(radPitch),
				cos(radPitch) * sin(radYaw)
			};
			POSITION_VECTOR = TARGET_VECTOR - direction * DIST_FROM_TARGET;
		}

	public:
		ArcBall(float fov, float near, float far, float aspect) :
			POSITION_VECTOR(glm::vec3(0.0f)),
			TARGET_VECTOR(glm::vec3(0.0f)),
			UP_VECTOR(glm::vec3(0.0, 1.0, 0.0)),
			DIST_FROM_TARGET(2.0f),
			FIELD_OF_VIEW(fov),
			YAW(-90.0f),
			PITCH(0.0f)
		{
			PERSPECTIVE_MAT = glm::perspective(FIELD_OF_VIEW, aspect, near, far);
			RecalcPos();
		}

		~ArcBall() = default;

		void Update(GLFWwindow* window) {
			static double lastX = 0.0, lastY = 0.0;
			double mouseX, mouseY;
			glfwGetCursorPos(window, &mouseX, &mouseY);

			float dx = static_cast<float>(mouseX - lastX);
			float dy = static_cast<float>(mouseY - lastY);
			lastX = mouseX;
			lastY = mouseY;

			if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
				ProcMouseOrbit(dx, dy);

			if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
				ProcMousePan(dx * a_pan_speed, dy*a_pan_speed);
		}

		void setScrollDelta(float scrollY) {
			ProcMouseScroll(scrollY * a_scroll_sensitivity);  // apply scroll directly
		}

		glm::vec3 getCamPos() const {
			return POSITION_VECTOR;
		}
		
		float* renderView() {
			VIEW_MAT = glm::lookAt(POSITION_VECTOR, TARGET_VECTOR, UP_VECTOR);
			return glm::value_ptr(VIEW_MAT);
		}

		float* getProjMatrix() {
			return glm::value_ptr(PERSPECTIVE_MAT);
		}
	};
}
