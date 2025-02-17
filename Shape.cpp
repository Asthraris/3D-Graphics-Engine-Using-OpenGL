#include "Shape.h"


#include <iostream>

Shape::Shape():num_verts(0),num_inds(0)
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
}

Shape::~Shape()
{
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &VAO);
}

void Shape::Update(terrain_VERTEX* vert, int nv, unsigned short* ind, int ni)
{
	vertices = vert;
	indices = ind;
	num_verts = nv;
	num_inds = ni;
}


void Shape::render()
{
	glBindVertexArray(VAO);
	//glDrawArrays(GL_TRIANGLES, 0, num_verts);
	glDrawElements(GL_TRIANGLES, num_inds, GL_UNSIGNED_SHORT, nullptr);
}

void Shape::send()
{
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, num_verts * sizeof(terrain_VERTEX), vertices, GL_STATIC_DRAW);


	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, num_inds * sizeof(unsigned short), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(terrain_VERTEX), (void*)offsetof(terrain_VERTEX, POS));
	glEnableVertexAttribArray(0);

	//abhi filhaal ke liye mene color ko implemant nhi kiya hai for terrain sinece i m dynamiccaly setting colour bahed on normal

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(terrain_VERTEX), (void*)offsetof(terrain_VERTEX, NORMAL));
	glEnableVertexAttribArray(1);

}
