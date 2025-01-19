#include "Shader.h"

#include <glad/glad.h>
//for loading for path
#include<fstream>
#include<sstream>

//for error correction & handling
#include <iostream>

void PrintError(unsigned int& vertId, unsigned int& fragId) {

	int Vertres, Fragres;
	glGetShaderiv(vertId, GL_COMPILE_STATUS, &Vertres);
	if (Vertres == GL_FALSE) {
		int length;
		glGetShaderiv(vertId, GL_INFO_LOG_LENGTH, &length);
		char* error = (char*)malloc(length * sizeof(char));
		glGetShaderInfoLog(vertId, length, &length, error);
		std::cout << "VERTEX-Source-ERROR: " << error << "\n";
	}
	glGetShaderiv(vertId, GL_COMPILE_STATUS, &Fragres);
	if (Fragres == GL_FALSE) {
		int length;
		glGetShaderiv(fragId, GL_INFO_LOG_LENGTH, &length);
		char* error = (char*)malloc(length * sizeof(char));
		glGetShaderInfoLog(fragId, length, &length, error);
		std::cout << "FRAGMNET-Source-ERROR: " << error << "\n";
	}
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

		fragId = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragId, 1, &fragstr, nullptr);
		glCompileShader(fragId);

		PrintError(vertId, fragId);
	}

	PROGRAM_ID = glCreateProgram();
	glAttachShader(PROGRAM_ID, vertId);
	//strores two exe together
	glAttachShader(PROGRAM_ID, fragId);

	//links this exe's to program
	glLinkProgram(PROGRAM_ID);

	glDeleteShader(vertId);
	glDeleteShader(fragId);
}

Shader::~Shader(){
	glDeleteProgram(PROGRAM_ID);
}

unsigned int Shader::Activate(){
	glUseProgram(PROGRAM_ID);

	return PROGRAM_ID;
}
