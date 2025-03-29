#pragma once

#include<glm/glm.hpp>
#include<memory>
#include"renderer/Transformation.h"
#include"Shape.h"

class Base_entity {
private:
	static unsigned int m_unique_counter ;
public:
	unsigned int m_entity_id;
	bool m_alive;
	std::shared_ptr<Shape> m_shape;
	Transformation m_transform;
	Base_entity() {
		m_entity_id = m_unique_counter;
		m_unique_counter++;
		m_alive = true;
	}
	// Add virtual destructor to make the class polymorphic
	virtual ~Base_entity() = default;

	void set_transform(const glm::mat4& global_trans) {
		m_transform.set_Model(global_trans);
	}
};

class Dynamic_entities :public Base_entity {
public:
	glm::vec3 m_velocity;
	glm::vec3 m_acceralation;
	bool m_obeys_gravity;

	Dynamic_entities(
		glm::vec3 init_velo = glm::vec3(0.0f),
		glm::vec3 acc = glm::vec3(0.0f),
		bool gralogic = false)
		:m_obeys_gravity(gralogic) {}
	
	void Update(float deltaTime, const float& gravity) {
		m_velocity += m_acceralation * deltaTime;
		if (m_obeys_gravity) {
			m_velocity.y -= gravity * deltaTime;
		}
		m_transform.move(m_velocity);
	}
};