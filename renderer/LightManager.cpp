#include "LightManager.h"

#include <glad/glad.h>


//DEBUG
#include <iostream>



void LightManager::DEB_printLight()
{
		for (int i = 0; i < NUM_LIGHTS; i++) {
			std::cout << "Light " << i << " -\nType: " << Lights[i].Type
				<< "\nPos: (" << Lights[i].Pos.x << ", " << Lights[i].Pos.y << ", " << Lights[i].Pos.z << ")\n"
				<< "Color: (" << Lights[i].Color.r << ", " << Lights[i].Color.g << ", " << Lights[i].Color.b << ")\n"
				<< "Intensity: " << Lights[i].Intensity << "\n";
		}
}

LightManager::LightManager():NUM_LIGHTS(0)
{
}
//phele se hi bata de ki itna limit me kaam karna hai
LightManager::LightManager(int maxLIGHT):MAX_LIGHTS(maxLIGHT),NUM_LIGHTS(0){
	glGenBuffers(1, &lightUBO);
	glBindBuffer(GL_UNIFORM_BUFFER, lightUBO);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(light) * MAX_LIGHTS, nullptr, GL_DYNAMIC_DRAW);
	glBindBufferRange(GL_UNIFORM_BUFFER, 0, lightUBO, 0, sizeof(light) * MAX_LIGHTS);
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, lightUBO);

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
	if (NUM_LIGHTS <= 0)return NUM_LIGHTS;
	Lights.pop_back();
	NUM_LIGHTS--;
	return NUM_LIGHTS;
}

void LightManager::UpdateLights()
{
	//DEB_printLight();
	if (Lights.size() > 0) {
		glBindBuffer(GL_UNIFORM_BUFFER, lightUBO);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, Lights.size() * sizeof(light), Lights.data());
	}
	/*light testLight;
	glGetBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(light), &testLight);
	std::cout << "UPLODED Light \n type: "  << testLight.Type
		<< "\nPos: (" << testLight.Pos.x << ", " << testLight.Pos.y << ", " << testLight.Pos.z << ")\n"
		<< "Color: (" << testLight.Color.r << ", " << testLight.Color.g << ", " << testLight.Color.b << ")\n"
		<< "Intensity: " << testLight.Intensity << "\n";
	std::cout << "Size of light struct: " << sizeof(light) << " bytes" << std::endl;*/

	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}
