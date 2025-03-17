#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>


enum LEVEL {
	unreal = 0,
	unity = 1,
	godot =2
};

struct envConfigUniform {
	bool fog_enable = true;
	float fog_density =0.05;
	float fog_color[3] = {1.0, 1.0, 1.0};
	float ambient =0.0;
	int num_lights =0;
	bool light_enable = true;

	bool operator==(const envConfigUniform& other) const {
		return fog_enable == other.fog_enable &&
			std::fabs(fog_density - other.fog_density) < 1e-4f &&  // Floating point comparison
			std::fabs(fog_color[0] - other.fog_color[0]) < 1e-4f &&
			std::fabs(fog_color[1] - other.fog_color[1]) < 1e-4f &&
			std::fabs(fog_color[2] - other.fog_color[2]) < 1e-4f &&
			std::fabs(ambient - other.ambient) < 1e-4f &&
			num_lights == other.num_lights &&
			light_enable == other.light_enable;
	}

};

class CONFIG{
private:
	unsigned int configUBO;
public:
	envConfigUniform env;
	float sky[3] = {0.0,0.0,0.0};
	int render_distance;
	int level_of_detail;
	CONFIG() {};
	CONFIG(LEVEL lvl) {
		switch (lvl)
		{
		case unreal:
			render_distance = 5;
			level_of_detail = 4;
			break;
		case unity:
			render_distance = 4;
			level_of_detail = 3;
			break;
		case godot:
			render_distance = 3;
			level_of_detail = 2;
		}

		//generating the uniform layout for storing setting data gpu ke liye
		glGenBuffers(1, &configUBO);
		glBindBuffer(GL_UNIFORM_BUFFER, configUBO);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(envConfigUniform), &env, GL_DYNAMIC_DRAW);
		glBindBufferRange(GL_UNIFORM_BUFFER, 0, configUBO, 0, sizeof(envConfigUniform));
		glBindBufferBase(GL_UNIFORM_BUFFER, 0, configUBO);
	}
	void check4Change() {
		static envConfigUniform GpuSidedata;
		if (!(GpuSidedata == env)) {
			GpuSidedata = env;
			
			glBindBuffer(GL_UNIFORM_BUFFER, configUBO);
			glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(envConfigUniform), &GpuSidedata);
			glBindBuffer(GL_UNIFORM_BUFFER, 0);


		}
	}
};

struct WINDOW {
	int width;
	int height;
	std::string name;
	GLFWwindow* ptr = nullptr;
	void construct() {
		ptr = glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr);
	}
};