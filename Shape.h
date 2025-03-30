#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <glad/glad.h>
//for using memcpy for faster copying vertex data to shape
#include <cstring>

#include "renderer/terrain/terrain_vertex.h"

struct VERTEX {
	glm::vec3 POS;
	glm::vec3 COLOR = glm::vec3(0.1, 0.5, 0.1);
	glm::vec3 NORMAL = glm::vec3(0.0);
	glm::vec2 TEX_COORDS;
};


class Shape {
private:
	//filhaal me sab public kar raha hu baad me isko abstract karunga
public:
	std::vector<VERTEX> vertices;
	std::vector<uint32_t> indices;
	Shape() {};
	Shape(const VERTEX* base,size_t num_verts,const uint32_t* base_inds, size_t num_inds) {
		vertices.resize(num_verts);
		indices.resize(num_inds);
		std::memcpy(vertices.data(), base, num_verts*sizeof(VERTEX));
		std::memcpy(indices.data(), base_inds, num_inds*sizeof(uint32_t));
	}
};





class Terrain_Shape
{
private:
	unsigned int VAO, VBO , EBO;
	terrain_VERTEX* vertices;
	unsigned short* indices;
	int num_verts;
	int num_inds;
public:
	int LOD;
	Terrain_Shape();
	~Terrain_Shape();
	void Update(terrain_VERTEX* vert,int nv,unsigned short* ind,int ni);
	void render();
	void send();
};

