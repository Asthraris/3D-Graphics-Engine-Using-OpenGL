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
#include "Config.h"

//for enhance
#include <memory>
//debug



	//add level of renderer acc to main we decide the level of lod and render_dis
class Renderer
{
private:
	std::unique_ptr<WINDOW> win;
	CONFIG settings;

	int temp_LIGHT_NUM =0;

	LightManager Aura;
	
	void IMGUI_INIT(GLFWwindow*);
	void IMGUI_RENDER(int);
	void IMGUI_DESTROY();
	

public:
	Renderer(LEVEL,std::unique_ptr <WINDOW>);
	~Renderer();
	void Run();
};

