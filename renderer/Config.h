#pragma once

struct envConfig {
	bool fog_enable;
	float fog_density;
	float fog_color[3];
	float ambient;
	float sky[3] = {0.0,0.0,0.0};
};

struct CONFIG{
	envConfig env;
	bool light;
	bool wire;
	int render_distance;
	int level_of_detail;
};