#pragma once

#include "Shape.h"
#include <memory>

std::shared_ptr<Shape> genCube() {
	VERTEX vertices[] = {
		// Positions         // Colors
		glm::vec3( - 0.5f, -0.5f, -0.5f),glm::vec3( 1.0f, 0.0f, 0.0f), // Front-bottom-left, Red
		glm::vec3(0.5f, -0.5f, -0.5f),glm::vec3(0.0f, 1.0f, 0.0f), // Front-bottom-right, Green
		glm::vec3(0.5f,  0.5f, -0.5f),glm::vec3(0.0f, 0.0f, 1.0f), // Front-top-right, Blue
		glm::vec3(-0.5f,  0.5f, -0.5f),glm::vec3(1.0f, 1.0f, 0.0f), // Front-top-left, Yellow

		glm::vec3(-0.5f, -0.5f,  0.5f),glm::vec3(1.0f, 0.0f, 1.0f), // Back-bottom-left, Magenta
		glm::vec3(0.5f, -0.5f,  0.5f),glm::vec3(0.0f, 1.0f, 1.0f), // Back-bottom-right, Cyan
		glm::vec3(0.5f,  0.5f,  0.5f),glm::vec3(1.0f, 1.0f, 1.0f), // Back-top-right, White
		glm::vec3(-0.5f,  0.5f,  0.5f),glm::vec3(0.0f, 0.0f, 0.0f)  // Back-top-left, Black
	};

	// Indices for the cube's faces (two triangles per face)
	unsigned short indices[] = {
		0, 1, 2,  2, 3, 0,   // Front face
		4, 5, 6,  6, 7, 4,   // Back face
		4, 5, 1,  1, 0, 4,   // Bottom face
		7, 6, 2,  2, 3, 7,   // Top face
		4, 0, 3,  3, 7, 4,   // Left face
		1, 5, 6,  6, 2, 1    // Right face
	};

std::shared_ptr<Shape> cub = std::make_shared<Shape>();
//this is used to craete a smart pointer =shared whch means more than one owners ,unique bhi ata hai

	cub->Update(vertices, sizeof(vertices) / sizeof(VERTEX), indices, sizeof(indices) / sizeof(unsigned short));
	cub->send();

	return cub;
}