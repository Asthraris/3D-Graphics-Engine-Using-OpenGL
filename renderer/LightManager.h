#pragma once

#include <vector>
#include <glm/glm.hpp>

struct light {
	//type 0-Dir 1-Spot 2-Point
	//filhaal iski zarurat nhi hai
	int Type;
	glm::vec3 Pos;//for dir Pos is Direction
	glm::vec3 Color;
	float Intensity;
};



class LightManager
{
private:
	int MAX_LIGHTS;
	std::vector <light> Lights;
	unsigned int lightUBO;
	void CreateLightBuffer();
public:
	int NUM_LIGHTS;
	LightManager();
	LightManager(int maxLIGHT);
	int AddLight(const light& LIGHT);
	int removeLight();

	void UpdateLights();
};

