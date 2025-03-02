#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <glad/glad.h>

#include "renderer/terrain/terrain_vertex.h"

class Shape
{
private:
	unsigned int VAO, VBO , EBO;
	terrain_VERTEX* vertices;
	unsigned short* indices;
	int num_verts;
	int num_inds;
public:
	int LOD,PERLIN;
	Shape();
	~Shape();
	void Update(terrain_VERTEX* vert,int nv,unsigned short* ind,int ni);
	void render();
	void send();



};

