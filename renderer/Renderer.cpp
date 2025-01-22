#include "Renderer.h"

#include "src/Shader.h"
#include "../Camera.h"


//for debug model mat
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

//for running -temp
#include "../Cube.h"
#include "../Terrain.h"


//problem ye aati thi ke func ke ander mai jo bhi obj banata tha ur uska reference kahinour ano.func me use karta tha tab tak destruct call hojata tha clearing that obj only pointer me adrress hota tha but not pointer 
//c/cpp me ye detect bhi nhi hota kyuki jo new address aata tha usse c typecast kardeta tha without checking the validity of that obj 
float Timer() {
	static double lastTime = 0.0;
	double currtime = glfwGetTime();
	float dt = float(currtime - lastTime);
	lastTime = currtime;
	return dt;
}

Renderer::Renderer(const int& width,const int& height, const char* winName):ACTIVE_SHADER(0){
	//OPENGL -4.5 version with directStateAccess
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);

	WIN_WIDTH = width;
	WIN_HEIGHT = height;
	window = glfwCreateWindow(WIN_WIDTH,WIN_HEIGHT,winName,nullptr,nullptr);
	glfwMakeContextCurrent(window);
	//glfw se humne glad procedure address liya fir typecast kiya to glad provided script then load kiya into glad
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	//for 3d rendering
	glEnable(GL_DEPTH_TEST);
	glViewport(0, 0, WIN_WIDTH, WIN_HEIGHT);
	glClearColor(SKY_COLOR[0], SKY_COLOR[1], SKY_COLOR[2], SKY_COLOR[3]);

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
	Shader simple("renderer/src/shaders/vertex.glsl", "renderer/src/shaders/fragment.glsl");
	

	ACTIVE_SHADER = simple.Activate();


	Camera cam(60.0f, 0.1f, 10.0f, float(WIN_WIDTH) / (float)WIN_HEIGHT);
	glm::mat4 Model = glm::mat4(1.0f);
	Model = glm::translate(Model, glm::vec3(0.0f, 0.0f, -3.0f));
	simple.DEB_ModelMatTest("Modle", Model);
	

	
	Meshes.push_back(genTerrain(8,8,0.1,2.0));
	//Meshes.push_back(genCube());
	
	
	while (!glfwWindowShouldClose(window)) {
		deltaTime = Timer();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		cam.Move(deltaTime, window);
		cam.Look(deltaTime, window);
		cam.renderView(simple);
	
		for (const auto& mesh:Meshes) {
			mesh->render();
		}
			
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}


