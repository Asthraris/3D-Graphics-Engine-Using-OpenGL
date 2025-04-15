#pragma once

#include <glm/glm.hpp>
#include <memory>

namespace eng{
	class Transformation {
	public:
		glm::vec3 Position;
		float Rotation_degree;
		glm::vec3 Rotational_axis;
		glm::vec3 Scale;

		glm::mat4* mat_ptr;
		Transformation():
			Position(glm::vec3(0.0f)),
			Rotation_degree(0.0),
			Rotational_axis(glm::vec3(0.0,1.0,0.0)),
			Scale(1.0f)
			{}
		glm::mat4 getModelMat() {
			glm::mat4 mod = glm::mat4(1.0);
			mod = glm::scale(mod, Scale);
			mod = glm::rotate(mod, Rotation_degree, Rotational_axis);
			mod = glm::translate(mod, Position);
			return mod;
		}

	};
}