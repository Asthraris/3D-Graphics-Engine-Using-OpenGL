#pragma once

#include <vector>
#include <glm/glm.hpp>

struct light {
	alignas(16) int Type =0;
	alignas(16) glm::vec3 Pos =glm::vec3(0.0);
	alignas(16) glm::vec3 Color = glm::vec3(1.0);
	alignas(16) float Intensity = 0.5;
};




class LightManager
{
private:
	int MAX_LIGHTS;
	std::vector <light> Lights;
	unsigned int lightUBO;
	void DEB_printLight();

public:
	int NUM_LIGHTS =0;
	LightManager();
	LightManager(int maxLIGHT);
	void AddLight(const light& LIGHT);
	void removeLight();

	void UpdateLights();
	void IMGUI_RENDER();
};

