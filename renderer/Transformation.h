#pragma once

#include<glm/glm.hpp>
class Transformation
{
private:
	glm::mat4 Model;
	glm::vec3 m_position;
public:
	Transformation();
	void set_Model(const glm::mat4& temp);
	void transform(glm::vec3);
	void move(glm::vec3);
	void enlarge(float);
	void distort(glm::vec3);
	void rotate(glm::vec3&, float);
	float* getMat();
};

