#include "Shader.h"

#include <glad/glad.h>
//for loading for path
#include<fstream>
#include<sstream>

//for cam matrix type converion

//for error correction & handling
#include <iostream>

void PrintError(unsigned int& souceId ) {

	int result;
	glGetShaderiv(souceId, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE) {
		int length;
		glGetShaderiv(souceId, GL_INFO_LOG_LENGTH, &length);
		char* error = (char*)malloc(length * sizeof(char));
		glGetShaderInfoLog(souceId, length, &length, error);
		std::cout << "Source-ERROR: " << error << "\n";
	}
}

Shader::Shader()
{
}

Shader::Shader(const char* Vertpath, const char* Fragpath){
	unsigned int vertId,fragId;
	{
		//yaha par hum some naam se file ko open kar rahe hai at that file path
		std::ifstream vert(Vertpath);
		if (!vert) {
			std::cout << "GYANI: VERT_PATH not found\n";
		}
		//ek string ko stram karne layak container bana te hai
		std::stringstream vertCache;
		//fir us container me rdbuf ke Ascii conversion karke storekarte hai
		vertCache << vert.rdbuf();
		//fir woh opened file ko close kardete hai
		vert.close();
		//joh temp string container se hum copy karte hai to new std::string var (Usable)
		std::string vertSource = vertCache.str();
		const char* vertstr = vertSource.c_str();

		std::ifstream frag(Fragpath);
		if (!frag) {
			std::cout << "GYANI: FRAG_PATH not found\n";
		}
		std::stringstream fragCache;
		fragCache << frag.rdbuf();
		frag.close();
		std::string fragSource = fragCache.str();
		const char* fragstr = fragSource.c_str();
		vertId = glCreateShader(GL_VERTEX_SHADER);
		//yaha par nullptr isliye kyuki ye pura string hi vert shader hai agar mene ek hi file me dono likha hota toh mera start alag hoga
		glShaderSource(vertId, 1, &vertstr, nullptr);
		//creates an executable file for gpu to execute
		glCompileShader(vertId);
		PrintError(vertId);

		fragId = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragId, 1, &fragstr, nullptr);
		glCompileShader(fragId);

		PrintError(fragId);
	}

	PROGRAM_ID = glCreateProgram();
	glAttachShader(PROGRAM_ID, vertId);
	//strores two exe together
	glAttachShader(PROGRAM_ID, fragId);

	//links this exe's to program
	glLinkProgram(PROGRAM_ID);


	

	glDeleteShader(vertId);
	glDeleteShader(fragId);

	glUseProgram(PROGRAM_ID);
	VIEW_MAT_LOC = glGetUniformLocation(PROGRAM_ID, "View_Matrix" );
	if (VIEW_MAT_LOC == -1)std::cout << " View not found\n";

	PROJ_MAT_LOC = glGetUniformLocation(PROGRAM_ID, "Proj_Matrix");
	if (PROJ_MAT_LOC == -1)std::cout << " PERS not found\n";

	MODEL_MAT_LOC = glGetUniformLocation(PROGRAM_ID, "Model_Matrix");
	if (MODEL_MAT_LOC == -1)std::cout << " MOdel not found\n";

	NUM_LIGHTS_LOC = glGetUniformLocation(PROGRAM_ID, "NUM_LIGHTS");
	if (NUM_LIGHTS_LOC == -1)std::cout << " NUM_lightnot found\n";
	

	LIGHT_BLOCK_LOC = glGetUniformBlockIndex(PROGRAM_ID, "LIGHTS");
	glUniformBlockBinding(PROGRAM_ID, LIGHT_BLOCK_LOC, 0);
	if (LIGHT_BLOCK_LOC == -1)std::cout << "LIGHts block not found\n";

	CONFIG_BLOCK_LOC = glGetUniformBlockIndex(PROGRAM_ID, "SETTINGS");
	glUniformBlockBinding(PROGRAM_ID, CONFIG_BLOCK_LOC, 1);
	if (CONFIG_BLOCK_LOC == -1)std::cout << "Config block not found\n";

	glUseProgram(0);
}

Shader::~Shader(){
	glUseProgram(0);
	glDeleteProgram(PROGRAM_ID);
}
void Shader::projMatrix(const float* Value)
{
	glUniformMatrix4fv(PROJ_MAT_LOC, 1, GL_FALSE, Value);
}
//kyuki me camera toh obviauslly banane wala hu so why not store the loc value in shader object
void Shader::viewMatrix(const float* Value)
{
	glUniformMatrix4fv(VIEW_MAT_LOC, 1, GL_FALSE,Value);
}

void Shader::UpdateModelMatrix(const float* Value)
{
	glUniformMatrix4fv(MODEL_MAT_LOC, 1, GL_FALSE, Value);
}

void Shader::UpdateNUM_LIGHTS(const int num_lights)
{
	glUniform1i(NUM_LIGHTS_LOC, num_lights);
}

void Shader::Activate(){
	glUseProgram(PROGRAM_ID);
}

unsigned int Shader::getProgram()
{
	return PROGRAM_ID;
}
