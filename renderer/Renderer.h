#pragma once
//required
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <../imgui/imgui.h>
#include <../imgui/imgui_impl_glfw.h>
#include <../imgui/imgui_impl_opengl3.h>
//utils
#include "src/Shader.h"
#include "LightManager.h"
#include "Config.h"
#include "../SceneManager.h"

//for enhance
#include <memory>
//debug



	//add level of renderer acc to main we decide the level of lod and render_dis
class Renderer
{
private:
	std::unique_ptr<WINDOW> win;
	CONFIG settings;
	LightManager Aura;

	Shader* SHADER;
	SceneManager* SCENE;
	std::shared_ptr<ComponentManager> RENDERER_Component_UNIT;
	//just to make renderer access data efficiently does need to fetch everytime from data
	void IMGUI_INIT(GLFWwindow*);
	void IMGUI_RENDER(int);
	void IMGUI_DESTROY();
	bool DEBUGfirstrun = true;

	/*--------------------Renderer stuffs -------------------*/
	size_t NUM_MERGED_CMD;
	//needed while we render 
	unsigned int MERGED_VAO, MERGED_MDI_COMMAND, MERGED_MODEL_SSBO;

	//for vertex buffer
	unsigned int VBO;

	//for index buffer
	unsigned int IBO;
	//i have decided i m going to use PERCISTNAT map
	int BINDIND_INDX_INST = 0;

	GLsizei sizeof_mdi_commands = sizeof(DrawElementsIndirectCommand);

public:
	Renderer(LEVEL,std::unique_ptr <WINDOW>);
	~Renderer();
	void Initialize(const std::shared_ptr<ComponentManager>& Component_UNIT);
	void Run();
	void finalMDIRender();
};

