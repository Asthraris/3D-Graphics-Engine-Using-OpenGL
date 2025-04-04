#include "Renderer.h"

#include "../Camera.h"


//for debug model mat
#include <iostream>
#include <glm/glm.hpp>

//for running -temp



//problem ye aati thi ke func ke ander mai jo bhi obj banata tha ur uska reference kahinour ano.func me use karta tha tab tak destruct call hojata tha clearing that obj only pointer me adrress hota tha but not pointer 
//c/cpp me ye detect bhi nhi hota kyuki jo new address aata tha usse c typecast kardeta tha without checking the validity of that obj 
float Timer() {
	static double lastTime = 0.0;
	double currtime = glfwGetTime();
	float dt = float(currtime - lastTime);
	lastTime = currtime;
	return dt;
}
void checkError() {
	GLenum err = glGetError();
	if (err != GL_NO_ERROR) {
		std::cout << "RENDERER-SLIDER ERROR: " << err << "\n";
	}
}

// Callback function for keyboard input
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	static bool wire_frame = false;
	static bool mouseHidden = false;
	// Close window when ESC is pressed
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	if (key == GLFW_KEY_2 && action == GLFW_PRESS) {
		wire_frame = !wire_frame;
		glPolygonMode(GL_FRONT_AND_BACK, wire_frame ? GL_LINE : GL_FILL);
		std::cout << "Wireframe Mode: " << (wire_frame ? "ON\n" : "OFF\n") ;
	}
	if (key == GLFW_KEY_1 && action == GLFW_PRESS) {
		mouseHidden = !mouseHidden;
		glfwSetInputMode(window, GLFW_CURSOR, mouseHidden ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
		std::cout << "Mouse Cursor: " << (mouseHidden ? "HIDDEN\n" : "VISIBLE\n");
	}
}


void Renderer::IMGUI_INIT(GLFWwindow* window_ptr)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;

	ImGui_ImplGlfw_InitForOpenGL(window_ptr, true);
	ImGui_ImplOpenGL3_Init("#version 460");
}

void Renderer::IMGUI_RENDER(int fps)
{
	// Start the ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	// Create your UI here
	//idhar hamne ek window ka segment create kiyawith name this {if want to create more window seg. copy from this}
	ImGui::Begin("3D Renderer By Aman Gupta!");
	ImGui::Text("fps meter: %d",fps);
	ImGui::ColorEdit3("bg-Color", settings.sky);
	ImGui::Text("Render distance:");
	ImGui::SliderInt("render", &settings.render_distance, 1, 10);
	ImGui::SliderInt("LOD", &settings.level_of_detail, 1, 10);
	ImGui::Text("ENVIORNMENT");
	ImGui::SliderFloat("Ambient Light", &settings.env.ambient, 0.0f, 1.0f);
	ImGui::Checkbox("Enable Lighting", (bool*)&settings.env.light_enable);
	ImGui::Checkbox("Enable Fog", (bool*)&settings.env.fog_enable);
	
	ImGui::ColorEdit3("Fog Color", &settings.env.fog_color.x);
	ImGui::SliderFloat("Fog Density", &settings.env.fog_density, 0.0f, 0.5f);
	


	//ImGui::SliderInt("PERLIN", &TERR_PER, 1, 10);
	




	ImGui::End();

	Aura.IMGUI_RENDER();

	// Render ImGui
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Renderer::IMGUI_DESTROY(){
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

Renderer::Renderer(LEVEL lvl,std::unique_ptr <WINDOW> ptr):win(std::move(ptr)) {
	//OPENGL -4.5 version with directStateAccess
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	
	win->construct();
	
	

	glfwMakeContextCurrent(win->ptr);
	//glfw se humne glad procedure address liya fir typecast kiya to glad provided script then load kiya into glad
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	//for 3d rendering
	glViewport(0, 0, win->width, win->height);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_ARB_draw_indirect);

	glCullFace(GL_BACK);

	IMGUI_INIT(win->ptr);
	//making vertex as point  no fragment is running

	settings = CONFIG(lvl);
	Aura = LightManager(3);

	SHADER = new Shader("renderer/src/shaders/basic.vert", "renderer/src/shaders/basic.frag");
	SCENE = new SceneManager();

	std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

}

Renderer::~Renderer()
{
	IMGUI_DESTROY();
	glfwDestroyWindow(win->ptr);
	glfwTerminate();
}

void Renderer::Run()
{
	float deltaTime;
	bool DEBUGfirstrun = true;

	Camera cam(60.0f, 0.1f, 100.0f, float(win->width) / (float)win->height );

	//Terrain riverland;
	// 
	//keyboard event listener 
	glfwSetKeyCallback(win->ptr, keyCallback);

		//SLIDER DEBUG METER
		if (DEBUGfirstrun)checkError();
		DEBUGfirstrun = false;
		//SLIDER DEBUG METER
	SCENE->createEntity(STATIC,"CUBE");
	
	


	SCENE->InitializeData();
	while (!glfwWindowShouldClose(win->ptr)) {
		deltaTime = Timer();
		//std::cout << 1.0/deltaTime << "\n";
		settings.check4Change();
		glClearColor(settings.sky[0], settings.sky[1], settings.sky[2], 1.0);
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		cam.Move(deltaTime, win->ptr);
		cam.Look(deltaTime, win->ptr);

		SHADER->Activate();
		SHADER->viewMatrix(cam.renderView());
		SHADER->projMatrix(cam.getProjMatrix());
		SHADER->UpdateNUM_LIGHTS(Aura.NUM_LIGHTS);


		SCENE->Update_Scene(deltaTime);
		SCENE->Render_Scene();

		
		//riverland.dynamicLoad(cam, Aura.NUM_LIGHTS, settings.render_distance, settings.level_of_detail);

		IMGUI_RENDER(int(1/deltaTime));
		glfwSwapBuffers(win->ptr);

		glfwPollEvents();


	}
}


