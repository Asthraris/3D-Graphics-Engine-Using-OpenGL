#pragma once
//required
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <../imgui/imgui.h>
#include <../imgui/imgui_impl_glfw.h>
#include <../imgui/imgui_impl_opengl3.h>
//utils
#include "src/Shader.hpp"
#include "Utils.hpp"
#include "../ArcBall.hpp"
#include "../LightManager.hpp"
#include "../SceneManager.hpp"
#include "../ComponentManager.hpp"

//debug



	//add level of renderer acc to main we decide the level of lod and render_dis
namespace rend{
	class Renderer
	{
	private:
		std::unique_ptr<WINDOW> a_win;
		CONFIG a_settings;
		LightManager a_aura;
		Shader* a_shader;
		eng::ArcBall* a_editCam;
		eng::SceneManager* a_scene;
		std::shared_ptr<eng::ComponentManager> a_component_UNIT;
		//just to make renderer access data efficiently does need to fetch everytime from data
		void IMGUI_INIT(GLFWwindow*);
		void IMGUI_RENDER(int);
		void IMGUI_DESTROY();
		bool DEBUGfirstrun = true;

		/*--------------------Renderer stuffs -------------------*/
		unsigned int NUM_MERGED_CMD;
		//needed while we render 
		unsigned int MERGED_VAO, MERGED_MDI_COMMAND, MERGED_MODEL_SSBO;

		//for vertex buffer
		unsigned int VBO;

		//for index buffer
		unsigned int IBO;
		//i have decided i m going to use PERCISTNAT map
		int BINDIND_INDX_INST = 0;

		GLsizei sizeof_mdi_commands = sizeof(DrawElementsIndirectCommand);

		//-------------
		static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
		void UpdateModelmatrixInGPU(int32_t changed_matrix_id);

	public:
		Renderer(LEVEL, std::unique_ptr <WINDOW>);
		~Renderer();
		void UpdateBuffers(const std::shared_ptr<eng::ComponentManager>& Component_UNIT);
		void Run();
		void finalMDIRender();
	};
}

