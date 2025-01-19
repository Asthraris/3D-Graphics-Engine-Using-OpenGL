#include "Renderer.h"

#include "src/Shader.h"
#include "../Camera.h"


//for debug model mat
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
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
	
	GLfloat vertices[] = {
		// Positions         // Colors
		-0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, // Front-bottom-left, Red
		 0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // Front-bottom-right, Green
		 0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // Front-top-right, Blue
		-0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 0.0f, // Front-top-left, Yellow

		-0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 1.0f, // Back-bottom-left, Magenta
		 0.5f, -0.5f,  0.5f, 0.0f, 1.0f, 1.0f, // Back-bottom-right, Cyan
		 0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, // Back-top-right, White
		-0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 0.0f  // Back-top-left, Black
	};

	// Indices for the cube's faces (two triangles per face)
	GLuint indices[] = {
		0, 1, 2,  2, 3, 0,   // Front face
		4, 5, 6,  6, 7, 4,   // Back face
		4, 5, 1,  1, 0, 4,   // Bottom face
		7, 6, 2,  2, 3, 7,   // Top face
		4, 0, 3,  3, 7, 4,   // Left face
		1, 5, 6,  6, 2, 1    // Right face
	};

	GLuint VAO, VBO, EBO;

		// Generate and bind VAO
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		// Generate and bind VBO
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		// Generate and bind EBO
		glGenBuffers(1, &EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		// Position attribute (location = 0)
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);

		// Color attribute (location = 1)
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);

	glBindVertexArray(VAO);

	while (!glfwWindowShouldClose(window)) {
		deltaTime = Timer();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		cam.Move(deltaTime, window);
		cam.Look(deltaTime, window);
		cam.renderView(simple);



		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		//glDrawArrays(GL_TRIANGLES, 0,3);

		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}


