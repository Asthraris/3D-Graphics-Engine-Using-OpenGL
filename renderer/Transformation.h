#pragma once

#include<glm/glm.hpp>
class Transformation
{
private:
	glm::mat4 Model;
public:
	Transformation();
	void transform(glm::vec3);
	void move(glm::vec3);
	void enlarge(float);
	void distort(glm::vec3);
	void rotate(glm::vec3&, float);
	float* getMat();
};

