#pragma once
//required
#include<glad/glad.h>
#include<GLFW/glfw3.h>
//debug

class Renderer
{
private:
	int WIN_WIDTH, WIN_HEIGHT;
	GLFWwindow* window;
	float SKY_COLOR[4] = {1.0,0.0,0.0,1.0};
	unsigned int ACTIVE_SHADER;
public:
	Renderer(const int& width,const int& height,const char* winName);
	~Renderer();
	void Init();
	void Run();
	//	void Update();

};

