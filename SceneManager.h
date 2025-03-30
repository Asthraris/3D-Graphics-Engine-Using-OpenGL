#pragma once

#include<vector>
#include"Entities.h"

struct scene_node {
	ENTITY entity;
	
	std::vector<scene_node*>Childrens;
	// abhi ke liye errror hai
	scene_node(ENTITY te):entity(te){}

	void addChild(scene_node* child) {
		Childrens.push_back(child);
	}
};

class SceneManager {
private:
	//no need to create an intance since every method is static in factory
	EntitiesIDGenerator id_generator;
	ComponentManager Component_UNIT;
	scene_node* e_Root;
public:
	SceneManager() {
		id_generator = EntitiesIDGenerator();
		Component_UNIT = ComponentManager();
	}
	void createEntity(entities_type l_type, scene_node* l_parent = nullptr) {
		ENTITY temp = { id_generator.create_id() ,l_type };
		Component_UNIT.markEntry(temp,)
	}
};

