#pragma once

#include <unordered_map>
#include <functional>//for std::hash
#include <memory>
#include <glm/glm.hpp>

#include <glad/glad.h>

//utils
#include "Chunk.h"
#include "../vertex.h"




struct Grid {
	int x, y;
	Grid(int a, int b) {
		x = a;
		y = b;
	}
	bool operator ==(const Grid& other)const {
		return x == other.x && y == other.y;
	}
};
struct GridHash_ex {
	size_t operator ()(const Grid& obj)const {
		return std::hash<int>()(obj.x) ^ (std::hash<int>()(obj.y) << 1);
	}
};

class Chunk_Render {
	unsigned int VAO, VBO;
public:
	Chunk_Render(unsigned int ibo) {
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, NUM_VERTICES * sizeof(VERTEX), NULL, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VERTEX), (void*)offsetof(VERTEX, POS));
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VERTEX), (void*)offsetof(VERTEX, COLOR));
		glEnableVertexAttribArray(1);
	}
	~Chunk_Render() {
		glDeleteBuffers(1, &VBO);
		glDeleteVertexArrays(1, &VAO);
	}
	void Update(const void* data ) {
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, NUM_VERTICES * sizeof(VERTEX), data);
	}
	void Render() {
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, NUM_INDICES, GL_UNSIGNED_SHORT, nullptr);
	}
};

class Terrain
{
	std::unordered_map<Grid, std::shared_ptr<HeightMap>, GridHash_ex> Map;
	std::unordered_map<Grid, std::shared_ptr<Chunk_Render>, GridHash_ex> Slots;
	int RENDER_DIST;
	unsigned int IBO;
public:

	Terrain(int render_distance);
	~Terrain();
	void DynamicLoad(glm::vec2 Cam_Chunk_Loc);

};

