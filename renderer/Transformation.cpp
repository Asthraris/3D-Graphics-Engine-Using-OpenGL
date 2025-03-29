#include "Transformation.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
Transformation::Transformation()
{
	Model = glm::mat4(1.0f);
	m_position = glm::vec3(0.0);
}

void Transformation::set_Model(const glm::mat4& temp)
{
	Model = temp;
}



void Transformation::transform(glm::vec3 update){
	m_position = update;
	Model = glm::translate(glm::mat4(1.0f), m_position);
}

void Transformation::move(glm::vec3 update)
{
	m_position += update;
	Model = glm::translate(Model, m_position);
}

void Transformation::enlarge(float size){
	for (int i = 0; i < 4; i++) {
		Model[i][i] = size;
	}
}

void Transformation::distort(glm::vec3 newshape)
{
	for (int i = 0; i < 4; i++) {
		Model[i][i] = newshape[i];
	}
}

void Transformation::rotate(glm::vec3& axis, float value)
{
	Model = glm::rotate(Model, value, axis);
}

float* Transformation::getMat()
{
	return glm::value_ptr(Model);
}
