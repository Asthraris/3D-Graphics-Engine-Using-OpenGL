#pragma once
//required
#include<glad/glad.h>
#include<GLFW/glfw3.h>
//utils
#include <unordered_map>
#include <functional>//for std::hash
#include <memory>
#include "../Shape.h"
//debug



struct Chunk {
	int x, y;
	Chunk(int a, int b) {
		x = a;
		y = b;
	}
	bool operator ==(const Chunk& other)const {
		return x == other.x && y == other.y;
	}
};
struct ChunkHash_ex {
	size_t operator ()(const Chunk& obj)const {
		return std::hash<int>()(obj.x) ^ (std::hash<int>()(obj.y) << 1);
	}
};
class Renderer
{
private:
	int WIN_WIDTH, WIN_HEIGHT;
	GLFWwindow* window;
	float SKY_COLOR[4] = {0.0,0.0,0.0,1.0};
	unsigned int ACTIVE_SHADER;
	//veryimp
	std::unordered_map<Chunk, std::shared_ptr<Shape> , ChunkHash_ex> Terrain;

public:
	Renderer(const int& width,const int& height,const char* winName);
	~Renderer();
	void Run();
	//	void Update();

};

