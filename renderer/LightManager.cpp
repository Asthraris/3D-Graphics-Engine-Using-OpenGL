#include "LightManager.h"

#include <glad/glad.h>


//DEBUG
#include <iostream>

LightManager::LightManager():NUM_LIGHTS(0)
{
}
//phele se hi bata de ki itna limit me kaam karna hai
LightManager::LightManager(int maxLIGHT):MAX_LIGHTS(maxLIGHT),NUM_LIGHTS(0){
	glGenBuffers(1, &lightUBO);
	glBindBuffer(GL_UNIFORM_BUFFER, lightUBO);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(light) * MAX_LIGHTS, nullptr, GL_DYNAMIC_DRAW);
	glBindBufferRange(GL_UNIFORM_BUFFER, 0, lightUBO, 0, sizeof(light) * MAX_LIGHTS);
}

int LightManager::AddLight(const light& LIGHT)
{
	if (NUM_LIGHTS == MAX_LIGHTS)return NUM_LIGHTS;
	Lights.push_back(LIGHT);
	NUM_LIGHTS++;
	return NUM_LIGHTS;
}

int LightManager::removeLight()
{
	if (NUM_LIGHTS < 0)return NUM_LIGHTS;
	Lights.pop_back();
	NUM_LIGHTS--;
	return NUM_LIGHTS;
}

void LightManager::UpdateLights()
{
	glBindBuffer(GL_UNIFORM_BUFFER, lightUBO);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, Lights.size() * sizeof(light), Lights.data());
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	std::cout << Lights.size();
}
