#pragma once
//required
#include<glad/glad.h>
#include<GLFW/glfw3.h>
//utils
#include <vector>
#include <memory>
#include "../Shape.h"
//debug

class Renderer
{
private:
	int WIN_WIDTH, WIN_HEIGHT;
	GLFWwindow* window;
	float SKY_COLOR[4] = {0.0,0.0,0.0,1.0};
	unsigned int ACTIVE_SHADER;
	//veryimp
	std::vector <std::shared_ptr<Shape>> Meshes;

public:
	Renderer(const int& width,const int& height,const char* winName);
	~Renderer();
	void Run();
	//	void Update();

};

