#pragma once

#include <vector>
#include <glm/glm.hpp>

struct light {
	alignas(16) int Type;
	alignas(16) glm::vec3 Pos;
	alignas(16) glm::vec3 Color;
	alignas(16) float Intensity;
};




class LightManager
{
private:
	int MAX_LIGHTS;
	std::vector <light> Lights;
	unsigned int lightUBO;
	void DEB_printLight();

public:
	int NUM_LIGHTS;
	LightManager();
	LightManager(int maxLIGHT);
	int AddLight(const light& LIGHT);
	int removeLight();

	void UpdateLights();
};

