#pragma once
#include <vector>
#include <functional>
#include <memory>
#include <type_traits> 
#include "Entities.h"


struct scene_Node {
	std::shared_ptr<Base_entity> Entity;
	glm::mat4 LocalTransform = glm::mat4(1.0f);
	glm::mat4 GlobalTransform = glm::mat4(1.0f);
	std::vector<std::unique_ptr<scene_Node>> Children;

	scene_Node(std::shared_ptr<Base_entity> n_ent) {
		Entity = n_ent;
	}
	void add_Child(std::unique_ptr<scene_Node> n_child) {
		Children.push_back(std::move(n_child));
	}
	void update(const glm::mat4& parentTransform, float deltaTime, float gravity) {
		GlobalTransform = parentTransform * LocalTransform;
		if (Entity) {
			Entity->set_transform(GlobalTransform);

			// Only call Update() if the entity is Dynamic_entities
			Dynamic_entities* dynamicEntity = dynamic_cast<Dynamic_entities*>(Entity.get());
			if (dynamicEntity) {
				dynamicEntity->Update(deltaTime, gravity);
			}
		}
		for (const auto& child : Children)
		{
			child->update(GlobalTransform,deltaTime,gravity);
		}
	}
};


class SceneManager
{
	std::unique_ptr <scene_Node> ROOT;
	SceneManager() {
		ROOT = std::make_unique<scene_Node>(nullptr);  // Root node without entity
	}

	void add_Entity(std::shared_ptr<Base_entity> entity, scene_Node* parent = nullptr) {
		auto newNode = std::make_unique<scene_Node>(entity);

		if (!parent) {
			ROOT->add_Child(std::move(newNode));
		}
		else {
			parent->add_Child(std::move(newNode));
		}
	}
	
	

};

