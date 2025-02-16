#pragma once

#include <unordered_map>
#include <functional>//for std::hash
#include <memory>
#include <glm/glm.hpp>
#include <glad/glad.h>

//utils

#include "../vertex.h"
#include "../../Shape.h"

/*
Environment	vec3 Base Color (RGB)	Description

Grassland	vec3(0.1, 0.5, 0.1)	Natural green
Desert		vec3(0.9, 0.7, 0.3)	Sand color
Snowy		vec3(0.9, 0.9, 0.9)	White snow
Rocky		vec3(0.5, 0.5, 0.5)	Gray stone
Volcanic	vec3(0.2, 0.1, 0.1)	Dark lava rock
*/


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

class Terrain
{
private:
	std::unordered_map<Grid, std::shared_ptr<Shape>, GridHash_ex> Map;
	glm::mat4 Model_Matrix;
	
public:
	Terrain();
	~Terrain();
	void dynamicLoad(glm::vec2 Cam_Chunk_Loc);
	float* getModelMatrix();

};

