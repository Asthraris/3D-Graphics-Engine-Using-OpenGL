#pragma once

#include <vector>
#include "Entities.h"
#include "renderer/src/Shader.h"
#include <memory>


//DEBUG



struct scene_node {
    ENTITY entity;
    std::vector<scene_node*> Childrens;  // Use raw pointers for performance

    scene_node(const ENTITY& te) : entity(te) {}

    void addChild(scene_node* child) {
        Childrens.push_back(child);
    }
};

class SceneManager {
private:
    EntitiesIDGenerator id_generator;
    std::shared_ptr<ComponentManager> Component_UNIT;
    scene_node* e_Root;
    Shader* SHADER;

    

public:
    SceneManager()
        : id_generator(), Component_UNIT(), e_Root(nullptr) {
        
        Component_UNIT = std::make_shared<ComponentManager>();
    }
    ~SceneManager() {
        deleteScene(e_Root);
    }
    // Create entity with raw pointers for performance
    void createEntity(entities_type l_type,std::string shapeName, size_t num_instnace = 1, scene_node* l_parent = nullptr ) {
        // Allocate scene_node on the stack for better performance
        ENTITY temp = ENTITY(id_generator.create_id(),l_type );
        if (l_parent != nullptr && l_parent->entity.type == DYNAMIC)temp.type = DYNAMIC;

        // Mark entry with temp entity
        Component_UNIT->markEntry(temp,shapeName,num_instnace);

        scene_node* new_node = new scene_node(temp);

        if (!e_Root) {
            e_Root = new_node;  // Set the root
        }
        else {
            l_parent->addChild(new_node);
        }
    }

    //Later JOB
    void Update_Scene(float deltaTime) {

    }
    std::shared_ptr<ComponentManager> getComponent2GPU() {
        return Component_UNIT;
    }
private:
    // Recursive function to free all nodes
    void deleteScene(scene_node* node) {
        if (!node) return;
        for (auto* child : node->Childrens) {
            deleteScene(child);
        }
        delete node;
    }

};
