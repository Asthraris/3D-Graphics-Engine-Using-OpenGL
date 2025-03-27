#pragma once

#include <unordered_map>
#include <functional>//for std::hash
#include <memory>

#include <glad/glad.h>

//utils
#include "../src/Shader.h"
#include "../vertex.h"
#include "../../Shape.h"
#include "../../Camera.h"
#include "../Transformation.h"

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
	std::unique_ptr<Transformation> trans = std::make_unique<Transformation>();
	Shader* SHADER;
	int BUBBLES = 8;
	float SEED;
public:
	Terrain();
	Terrain(float seed);
	~Terrain();
	void dynamicLoad(Camera& cam,const int& NUM_LIGHTS, const int& RenderDistance, const int& TERR_LOD);

};

