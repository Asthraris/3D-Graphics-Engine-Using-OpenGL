#include "Renderer.h"

#include "src/Shader.h"

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

	glClearColor(SKY_COLOR[0], SKY_COLOR[1], SKY_COLOR[2], SKY_COLOR[3]);
}

Renderer::~Renderer()
{
	glfwDestroyWindow(window);
	glfwTerminate();
}

void Renderer::Init()
{
	Shader simple("renderer/src/shaders/vertex.glsl", "renderer/src/shaders/fragment.glsl");
	const float CubeVertices[] = {
	0.5, 0.5, 0.5,
	-0.5, 0.5, 0.5,
	0.5, -0.5, 0.5,
	-0.5, -0.5, 0.5,

	0.5, 0.5, -0.5,
	-0.5, 0.5, -0.5,
	0.5, -0.5, -0.5,
	-0.5, -0.5, -0.5,
	};

	const unsigned short CubeIndexes[] = {
		0, 1, 2,
		1, 3, 2,

		4, 5, 0,
		5, 1, 0,

		7, 6, 5,
		6, 4, 5,

		3, 7, 2,
		7, 6, 2,

		2, 6, 0,
		6, 4, 0,

		1, 3, 5,
		3, 7, 5,
	};

	unsigned int  VAO, VBO, VIO;

	glCreateVertexArrays(1, &VAO);

	glCreateBuffers(1, &VBO);
	glCreateBuffers(1, &VIO);

	glNamedBufferStorage(VBO, sizeof(CubeVertices), CubeVertices, 0);
	glNamedBufferStorage(VIO, sizeof(CubeIndexes), CubeIndexes, 0);

	glVertexArrayVertexBuffer(VAO, 0, VBO, 0, 3 * sizeof(float));
	//ye specify karta hai that location of attrib POS=0,3rd no parameter is for which no of binded buffer is this to that VAO
	glVertexArrayAttribBinding(VAO, 0, 0);
	glVertexArrayAttribFormat(VAO, 0, 3, GL_FLOAT, GL_FALSE, 0);
	glEnableVertexArrayAttrib(VAO, 0);
	glVertexArrayElementBuffer(VAO, VIO);

	ACTIVE_SHADER = simple.Activate();
	glBindVertexArray(VAO);
	
}

//keeps window opened till we close	[ remember it runs for many times ]
void Renderer::Run()
{
	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);


		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}


