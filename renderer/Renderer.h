#pragma once
//required
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include <glm/glm.hpp>
//utils


//debug



class Renderer
{
private:
	int WIN_WIDTH, WIN_HEIGHT;
	GLFWwindow* window;
	float SKY_COLOR[4] = {0.5,0.5,7.0,1.0};
	
	int RENDER_DISTANCE;

public:
	Renderer(const int& width,const int& height,const char* winName);
	~Renderer();
	void Run();
	//	void Update();

};

