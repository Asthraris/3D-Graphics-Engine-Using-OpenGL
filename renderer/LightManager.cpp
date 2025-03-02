#include "LightManager.h"

#include <glad/glad.h>
void LightManager::CreateLightBuffer()
{
	glGenBuffers(1, &lightUBO);
	glBindBuffer(GL_UNIFORM_BUFFER, lightUBO);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(light) * MAX_LIGHTS,nullptr,GL_DYNAMIC_DRAW);
	glBindBufferRange(GL_UNIFORM_BUFFER, 1, lightUBO, 0, sizeof(light) * MAX_LIGHTS);
}
LightManager::LightManager():NUM_LIGHTS(0)
{
}
//phele se hi bata de ki itna limit me kaam karna hai
LightManager::LightManager(int maxLIGHT):MAX_LIGHTS(maxLIGHT),NUM_LIGHTS(0){}

void LightManager::AddLight(const light& LIGHT)
{
	Lights.push_back(LIGHT);
	NUM_LIGHTS++;
}

void LightManager::removeLight()
{
	Lights.pop_back();
	NUM_LIGHTS--;
}

void LightManager::UpdateLights()
{
	glBindBuffer(GL_UNIFORM_BUFFER, lightUBO);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, Lights.size() * sizeof(light), Lights.data());
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	
}
