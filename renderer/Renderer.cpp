#include "Renderer.h"

#include "terrain/Terrain.h"
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
		std::cout << err << "\n";
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


void Renderer::IMGUI_INIT(GLFWwindow* window)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 450");
}

void Renderer::IMGUI_RENDER(const float&fps)
{
	// Start the ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	// Create your UI here
	//idhar hamne ek window ka segment create kiyawith name this {if want to create more window seg. copy from this}
	ImGui::Begin("3D Renderer By Aman Gupta!");
	ImGui::Text("fps meter");
	ImGui::ColorEdit3("bg-Color", temp_SKY_COLOR);
	ImGui::Text("Render distance:");
	ImGui::SliderInt("render", &RENDER_DISTANCE, 1, 10);
	ImGui::SliderInt("LOD", &TERR_LOD, 1, 10);
	ImGui::SliderInt("PERLIN", &TERR_PER, 1, 10);


	ImGui::End();


	// Render ImGui
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Renderer::IMGUI_DESTROY(){
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

bool Renderer::Compare_Sky_Color(const float main[3], const float change[3])
{
	return ((main[0] == change[0]) && (main[1] == change[1]) && (main[2] == change[2]));
}

Renderer::Renderer(const int& width,const int& height, const char* winName):RENDER_DISTANCE(3),TERR_LOD(2),TERR_PER(4){
	//OPENGL -4.5 version with directStateAccess
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	WIN_WIDTH = width;
	WIN_HEIGHT = height;
	
	window = glfwCreateWindow(WIN_WIDTH,WIN_HEIGHT,winName,nullptr,nullptr);
	glfwMakeContextCurrent(window);
	//glfw se humne glad procedure address liya fir typecast kiya to glad provided script then load kiya into glad
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	//for 3d rendering
	glViewport(0, 0, WIN_WIDTH, WIN_HEIGHT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	IMGUI_INIT(window);
	//making vertex as point  no fragment is running
}

Renderer::~Renderer()
{
	glfwDestroyWindow(window);
	glfwTerminate();
}

void Renderer::Run()
{
	float deltaTime;
	bool firstrun = true;

	Camera cam(60.0f, 0.1f, 100.0f, float(WIN_WIDTH) / (float)WIN_HEIGHT );
	Terrain basic;
	//keyboard event listener 
	glfwSetKeyCallback(window, keyCallback);

	while (!glfwWindowShouldClose(window)) {
		deltaTime = Timer();
		std::cout << 1.0/deltaTime << "\n";
		if (!Compare_Sky_Color(SKY_COLOR, temp_SKY_COLOR)) {
			for (int i = 0; i < 3; i++){
				SKY_COLOR[i] = temp_SKY_COLOR[i];
			}
			glClearColor(SKY_COLOR[0], SKY_COLOR[1], SKY_COLOR[2], 1.0);
		}
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		cam.Move(deltaTime, window);
		cam.Look(deltaTime, window);

		//SLIDER DEBUG METER
		if (firstrun)checkError();
		firstrun = false;
		//SLIDER DEBUG METER
		
		basic.dynamicLoad(cam.renderView(),cam.giveCamChunk(),RENDER_DISTANCE,TERR_LOD,TERR_PER);

		IMGUI_RENDER(1.0/deltaTime);
		glfwSwapBuffers(window);
		glfwPollEvents();

	}
	IMGUI_DESTROY();
}


