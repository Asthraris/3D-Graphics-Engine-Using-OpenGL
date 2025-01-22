#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/noise.hpp>
#include <vector>
#include <memory>
#include <glad/glad.h>

#include "Shape.h"

//less the perlin intensity less pointy mess [flat=0.0] no upper limit
std::shared_ptr<Shape> genTerrain(int MAX_X=2, int MAX_Z=2,float steps=0.5,float perlin_intensity=0.2) {
	unsigned int VAO , VBO , EBO;
	
//heght map generation

std::vector	<VERTEX> vertices;
std::vector <unsigned short > indices;
//vertices generation


int tile_x = ceil(MAX_X / steps) + 1;
int tile_z = ceil(MAX_Z / steps) + 1;
//COLOUM MAJOR
vertices.resize(tile_x * tile_z);
int index = 0;
	for (float i = 0; i <= MAX_X; i+=steps) {
		for (float j = 0; j <= MAX_Z; j+=steps) {
			vertices[index++] = {	glm::vec3(i, glm::perlin(glm::vec2(i*perlin_intensity,j*perlin_intensity)),j),glm::vec3(1.0,0.0,0.0)};
		}
	}
//indexes generation
	indices.resize((ceil(MAX_X / steps)) * (ceil(MAX_Z / steps)) * 6);
	index = 0;
	int top_left, top_right, bot_left, bot_right;
	for (int i = 0; i < tile_x-1; i++) {
		for (int j = 0; j < tile_z-1; j++) {
			bot_left = i * (tile_z) + j;
			bot_right = bot_left + 1;
			top_left = bot_left + (tile_z);
			top_right = top_left + 1;

			// First triangle
			indices[index++] = bot_left;
			indices[index++] = top_left;
			indices[index++] = bot_right;

			// Second triangle
			indices[index++] = bot_right;
			indices[index++] = top_left;
			indices[index++] = top_right;
		}
	}
	//iske jagah par me heap se obj bhi bana sakta hu using heap
	std::shared_ptr<Shape> terr = std::make_shared<Shape>();
	terr->Update(vertices.data(),vertices.size(), indices.data(),indices.size());
	terr->send();
	
	return terr;
}
