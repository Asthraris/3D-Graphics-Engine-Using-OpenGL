#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <glad/glad.h>
//for using memcpy for faster copying vertex data to shape
#include <cstring>

struct VERTEX {
	glm::vec3 POS;
	glm::vec3 COLOR = glm::vec3(0.8, 0.8, 0.8);
	glm::vec3 NORMAL = glm::vec3(0.0);
	glm::vec2 TEX_COORDS = glm::vec2(0.0);  // Ensuring proper initialization

	// Default constructor
	VERTEX() = default;
	VERTEX(glm::vec3 pos, glm::vec3 color, glm::vec3 normal, glm::vec2 tex)
		: POS(pos), COLOR(color), NORMAL(normal), TEX_COORDS(tex) {}
};


class Shape {
private:
	//filhaal me sab public kar raha hu baad me isko abstract karunga
public:
	std::vector<VERTEX> vertices;
	std::vector<unsigned short> indices;
	Shape() {
		
	}

	// Copy constructor (fixed, no unsafe memcpy)
	Shape(const Shape& storedData) {
		vertices = storedData.vertices; // Direct copy
		indices = storedData.indices;   // Direct copy

	}
	void setMeshData(const std::vector<glm::vec3>& positions,
		const std::vector<glm::vec3>& normals,
		const std::vector<unsigned short>& indicesInput)
	{
		vertices.clear();
		indices = indicesInput;

		size_t count = positions.size();
		vertices.reserve(count);

		for (size_t i = 0; i < count; ++i) {
			glm::vec3 pos = positions[i];
			glm::vec3 normal = (i < normals.size()) ? normals[i] : glm::vec3(0.0f);
			glm::vec2 tex = glm::vec2(0.0f); // Currently not loaded

			vertices.emplace_back(pos,glm::vec3(1.0f), normal, tex);
		}
	}

};







