#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <glad/glad.h>

#include "renderer/vertex.h"

class Shape
{
private:
	unsigned int VAO, VBO , EBO;
	VERTEX* vertices;
	unsigned short* indices;
	int num_verts;
	int num_inds;
public:
	Shape();
	~Shape();
	void Update(VERTEX* vert,int nv,unsigned short* ind,int ni);
	void render();
	void send();



};

