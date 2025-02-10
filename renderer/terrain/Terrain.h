#pragma once

#include <unordered_map>
#include <functional>//for std::hash
#include <memory>
#include <glm/glm.hpp>

#include <glad/glad.h>

//utils

#include "../vertex.h"
#include "../../Shape.h"




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
	
public:

	~Terrain();
	void dynamicLoad(glm::vec2 Cam_Chunk_Loc);

};

