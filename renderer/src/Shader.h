#pragma once
class Shader
{
private:
	unsigned int PROGRAM_ID;
public:
	Shader(const char* Vertpath,const char* Fragpath);
	~Shader();
	unsigned int Activate();
};

