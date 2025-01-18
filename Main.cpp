#include <glad/glad.h>
#include <GLFW/glfw3.h>

//shader
#include <iostream>


int main() {
	//OPENGL -4.5 version with directStateAccess
	glfwInit();
	//for giving hint to glfw that we are using core opengl-4.5 
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//create a window using opengl
	GLFWwindow* window = glfwCreateWindow(780, 480, "Asthraris", nullptr, nullptr);
	//make glfw context we set above in that window to be ready use opengl-4.5
	glfwMakeContextCurrent(window);
	//initizing or getting pointers to Opengl Functions to be use
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	//setting clear color Ideal for Sparse env not good if we are using Voxel based or SKYBOX

	const char* vertSource =
		" #version 450 core\n"
		"layout(location=0)in vec3 vPos;\n"
		"void main(){\n"
		"gl_Position = vec4(vPos,1.0);\n"
		"}\n";
	const char* fragSource =
		" #version 450 core\n"
		"out vec4 Output;\n"
		"void main(){\n"
		"Output = vec4(1.0,1.0,1.0,1.0);\n"
		"}\n";

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

	unsigned int progId,VAO,VBO,VIO;


	{
		unsigned int vertId;
		unsigned int fragId;

		vertId = glCreateShader(GL_VERTEX_SHADER);
		fragId = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(vertId, 1, &vertSource, nullptr);
		glShaderSource(fragId, 1, &fragSource, nullptr);

		glCompileShader(vertId);
		glCompileShader(fragId);

		progId = glCreateProgram();
		glAttachShader(progId, vertId);
		glAttachShader(progId, fragId);
		glLinkProgram(progId);

		glDeleteShader(vertId);
		glDeleteShader(fragId);

	}
	glCreateVertexArrays(1, &VAO);

	glCreateBuffers(1, &VBO);
	glCreateBuffers(1, &VIO);

	glNamedBufferStorage(VBO, sizeof(CubeVertices), CubeVertices, 0);
	glNamedBufferStorage(VIO, sizeof(CubeIndexes), CubeIndexes, 0);

	glVertexArrayVertexBuffer(VAO, 0, VBO, 0, 3 * sizeof(float));
	//ye specify karta hai that location of attrib POS=0,3rd no parameter is for which no of binded buffer is this to that VAO
	glVertexArrayAttribBinding(VAO, 0, 0);
	glVertexArrayAttribFormat(VAO, 0, 3, GL_FLOAT, GL_FALSE, 0);
	glEnableVertexArrayAttrib(VAO,0);
	glVertexArrayElementBuffer(VAO, VIO);

	glUseProgram(progId);
	glBindVertexArray(VAO);
	//keeps window opened till we close	[ remember it runs for many times ]
	const float color[4] = {0.5,1.0,0.5,1.0};
	glEnable(GL_DEPTH_TEST);
	while (!glfwWindowShouldClose(window)) {
		//clears the window with clear color
		glClearBufferfv(GL_COLOR | GL_DEPTH, 0, color);//yaha par kuch zhol hai
		glDrawElements(GL_TRIANGLES, sizeof(CubeIndexes) / sizeof(unsigned short), GL_UNSIGNED_SHORT, 0);









		//swaps finished drawn frame to curr frame
		glfwSwapBuffers(window);
		//enable polling events like move minimize,maximize close the window
		glfwPollEvents();
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}