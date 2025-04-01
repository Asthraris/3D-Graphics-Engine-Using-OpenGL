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
  

    size_t NUM_MERGED_COMM;
    //needed while we render 
    unsigned int MERGED_VAO, MERGED_MDI_COMMAND, MERGED_MODEL_SSSB;
    //i have decided i m going to use PERCISTNAT map
    



public:
    SceneManager()
        : id_generator(), Component_UNIT(), e_Root(nullptr) {
        //unlike glgenvertexarray it creates and binda automatically
        //for required now but using DSA method
        glCreateVertexArrays(1, &MERGED_VAO);
        

    }
    ~SceneManager() {
        delete e_Root;  // Free the root
    }
    // Create entity with raw pointers for performance
    void createEntity(entities_type l_type, scene_node* l_parent = nullptr , size_t num_instnace =1) {
        // Allocate scene_node on the stack for better performance
        ENTITY temp = ENTITY(id_generator.create_id(),l_type );
        if (l_parent->entity.type == DYNAMIC && num_instnace== 1)temp.type = DYNAMIC;

        // Mark entry with temp entity
        Component_UNIT.markEntry(temp,"CUBE",num_instnace);

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

    //Later JOB
    void Update_Scene() {

    }
    void Render_Scene() {
        static size_t sizeof_mdi_commands= sizeof(MDI_commands);
        glBindVertexArray(MERGED_VAO);
        glBindBuffer(GL_DRAW_INDIRECT_BUFFER, MERGED_MDI_COMMAND);
        glBindBufferBase(GL_SHADER_STORAGE_BLOCK,)
        glMultiDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_SHORT, nullptr, NUM_STATIC_COMM, sizeof_mdi_commands);
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

    void UploadData() {
        //baadme isse public private dekhna abhi public maanke chal

        size_t TOTAL_VERTEX = Component_UNIT.next_MERGED_MDI_CMD.base_vertex;
        size_t TOTAL_INDEX = Component_UNIT.next_MERGED_MDI_CMD.base_index;

        std::vector<VERTEX> vertices(TOTAL_VERTEX);
        // Ensure the vector has enough space, if base_vertex is calculated incorrectly
        if (vertices.size() < TOTAL_VERTEX) {
            vertices.resize(TOTAL_VERTEX);
        }

        std::vector<unsigned short>indices(TOTAL_INDEX);
        if (indices.size() < TOTAL_INDEX) {
            indices.resize(TOTAL_INDEX);
        }

        size_t offset_verts = 0;
        size_t vert_size_bytes;

        size_t offset_inds = 0;
        size_t inds_size_bytes;

        for (auto& shape_shr : Component_UNIT.STORAGE.Shape_map) {

            vert_size_bytes = shape_shr->vertices.size() * sizeof(VERTEX);
            inds_size_bytes = shape_shr->indices.size() * sizeof(unsigned short);
            //if memeory error use std::copy
            memcpy(reinterpret_cast<char*>(vertices.data()) + offset_verts,
                shape_shr->vertices.data(), vert_size_bytes);
            memcpy(reinterpret_cast<char*>(indices.data()) + offset_inds,
                shape_shr->indices.data(), inds_size_bytes);

            offset_verts += vert_size_bytes;
            offset_inds += inds_size_bytes;

        }

        //for vertex buffer
        unsigned int VBO;

        //for index buffer
        unsigned int IBO;


        //binding the vAO before creation so i dont need to bind buffer again
        glBindVertexArray(STATIC_VAO);
        glCreateBuffers(1, &VBO);
        glNamedBufferData(VBO,TOTAL_VERTEX *sizeof(VERTEX), vertices.data(), GL_STATIC_DRAW); // OpenGL 4.5
        //linking not sure where this should be

        //ek baar hi set karnega as bining index 0 par ab kaam hoga with stride as vertexsize
        glVertexArrayVertexBuffer(STATIC_VAO, 0, VBO, 0, sizeof(VERTEX)); // OpenGL 4.5

        
        glEnableVertexArrayAttrib(STATIC_VAO, 0);
        glVertexArrayAttribFormat(STATIC_VAO, 0, 3, GL_FLOAT, GL_FALSE, offsetof(VERTEX, POS));
        glVertexArrayAttribBinding(STATIC_VAO, 0, 0);

        glEnableVertexArrayAttrib(STATIC_VAO, 1);
        glVertexArrayAttribFormat(STATIC_VAO, 1, 3, GL_FLOAT, GL_FALSE, offsetof(VERTEX, COLOR));
        glVertexArrayAttribBinding(STATIC_VAO, 1, 0);

        glEnableVertexArrayAttrib(STATIC_VAO, 2);
        glVertexArrayAttribFormat(STATIC_VAO, 2, 3, GL_FLOAT, GL_FALSE, offsetof(VERTEX, NORMAL));
        glVertexArrayAttribBinding(STATIC_VAO, 2, 0);

        glEnableVertexArrayAttrib(STATIC_VAO, 3);
        glVertexArrayAttribFormat(STATIC_VAO, 3, 2, GL_FLOAT, GL_FALSE, offsetof(VERTEX, TEX_COORDS));
        glVertexArrayAttribBinding(STATIC_VAO, 3, 0);

        //link Index buffer to VAO
        glCreateBuffers(1, &IBO);
        glNamedBufferData(IBO, TOTAL_INDEX * sizeof(unsigned short), indices.data(), GL_STATIC_DRAW);
        glVertexArrayElementBuffer(STATIC_VAO, IBO);

        //link Commandbuffer to VAO

        NUM_STATIC_COMM = Component_UNIT.static_entities_data.indirect_commands.size();
        glCreateBuffers(1, &STATIC_MDI_COMMAND);
        glNamedBufferData(STATIC_MDI_COMMAND, sizeof(MDI_commands) * NUM_STATIC_COMM,
            Component_UNIT.static_entities_data.indirect_commands.data(), GL_STATIC_DRAW);
        
    }
};
