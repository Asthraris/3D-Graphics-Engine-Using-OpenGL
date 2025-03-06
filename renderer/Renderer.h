#pragma once
//required
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <../imgui/imgui.h>
#include <../imgui/imgui_impl_glfw.h>
#include <../imgui/imgui_impl_opengl3.h>
//utils
#include "LightManager.h"

//debug



class Renderer
{
private:
	int WIN_WIDTH, WIN_HEIGHT;
	GLFWwindow* window;
	float SKY_COLOR[3] = {0.0,0.0,0.0};

	//for dynamic update
	float temp_SKY_COLOR[3]= { 0.82,0.63,1.0 };
	int temp_LIGHT_NUM =0;
	
	int RENDER_DISTANCE;//ye implement krana baaki hai
	int TERR_LOD, TERR_PER;

	LightManager Aura;
	
	void IMGUI_INIT(GLFWwindow* window);
	void IMGUI_RENDER(int fps);
	void IMGUI_DESTROY();


	bool Compare_Sky_Color(const float main[3], const float change[3]);
	

public:
	Renderer(const int& width,const int& height,const char* winName);
	~Renderer();
	void Run();

	//	void Update();

};

