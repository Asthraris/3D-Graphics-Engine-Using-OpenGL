#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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
		Transformation(const glm::mat4& guide) {
			
				// Extract translation
				Position = glm::vec3(guide[3]);

				// Extract scale (lengths of the columns of the upper-left 3x3 matrix)
				glm::vec3 col0 = glm::vec3(guide[0]);
				glm::vec3 col1 = glm::vec3(guide[1]);
				glm::vec3 col2 = glm::vec3(guide[2]);

				Scale.x = glm::length(col0);
				Scale.y = glm::length(col1);
				Scale.z = glm::length(col2);

				// Remove scale from the rotation matrix
				glm::mat3 rotMatrix;
				rotMatrix[0] = col0 / Scale.x;
				rotMatrix[1] = col1 / Scale.y;
				rotMatrix[2] = col2 / Scale.z;

				// Convert rotation matrix to quaternion
				glm::quat rotQuat = glm::quat_cast(rotMatrix);

				// Convert quaternion to axis-angle
				float angle_rad = glm::angle(rotQuat);
				glm::vec3 axis = glm::axis(rotQuat);

				// Store rotation
				Rotation_degree = glm::degrees(angle_rad);
				Rotational_axis = axis;
			

		}
		glm::mat4 getModelMat() {
			glm::mat4 mod = glm::mat4(1.0);
			mod = glm::scale(mod, Scale);
			mod = glm::rotate(mod, Rotation_degree, Rotational_axis);
			mod = glm::translate(mod, Position);
			return mod;
		}

	};
}