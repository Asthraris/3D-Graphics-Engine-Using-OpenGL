#pragma once

#include <vector>
#include "Entities.h"

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
    ComponentManager Component_UNIT;
    scene_node* e_Root;

public:
    SceneManager()
        : id_generator(), Component_UNIT(), e_Root(nullptr) {
    }
    ~SceneManager() {
        delete e_Root;  // Free the root
    }
    // Create entity with raw pointers for performance
    void createEntity(entities_type l_type, scene_node* l_parent = nullptr) {
        // Allocate scene_node on the stack for better performance
        ENTITY temp = ENTITY(id_generator.create_id(), l_type);

        // Mark entry with temp entity
        Component_UNIT.markEntry(temp,"CUBE");

        scene_node* new_node = new scene_node(temp);

        if (!e_Root) {
            e_Root = new_node;  // Set the root
        }
        else {
            l_parent->addChild(new_node);
        }
    }

    // Destructor to prevent memory leaks
    ~SceneManager() {
        deleteScene(e_Root);
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
