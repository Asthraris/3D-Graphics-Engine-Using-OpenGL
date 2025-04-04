#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <glad/glad.h>
//for using memcpy for faster copying vertex data to shape
#include <cstring>
#include <iostream>

struct VERTEX {
	glm::vec3 POS;
	glm::vec3 COLOR = glm::vec3(0.1, 0.5, 0.1);
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
		std::cout << "Shape is Empty!\n";
	}

	// Copy constructor (fixed, no unsafe memcpy)
	Shape(const Shape& storedData) {
		vertices = storedData.vertices; // Direct copy
		indices = storedData.indices;   // Direct copy

	}
};







