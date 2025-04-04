#pragma once

#include <vector>
#include "Entities.h"
#include "renderer/src/Shader.h"


//DEBUG
#include <iostream>
#define GL_CHECK_ERROR() \
    { \
        GLenum err; \
        while ((err = glGetError()) != GL_NO_ERROR) { \
            std::cerr << "OpenGL Error " << err << " at " << __FILE__ << ":" << __LINE__ << std::endl; \
        } \
    }


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
    Shader* SHADER;

    size_t NUM_MERGED_CMD;
    //needed while we render 
    unsigned int MERGED_VAO, MERGED_MDI_COMMAND, MERGED_MODEL_SSBO;

    //for vertex buffer
    unsigned int VBO;

    //for index buffer
    unsigned int IBO;
    //i have decided i m going to use PERCISTNAT map
    int BINDIND_INDX_INST = 0;

    GLsizei sizeof_mdi_commands = sizeof(DrawElementsIndirectCommand);

public:
    SceneManager()
        : id_generator(), Component_UNIT(), e_Root(nullptr) {
        //unlike glgenvertexarray it creates and binda automatically
        //for required now but using DSA method
        glCreateVertexArrays(1, &MERGED_VAO);

    }
    ~SceneManager() {
        deleteScene(e_Root);
        glDeleteVertexArrays(1, &MERGED_VAO);  // Free the root
    }
    // Create entity with raw pointers for performance
    void createEntity(entities_type l_type,std::string shapeName, size_t num_instnace = 1, scene_node* l_parent = nullptr ) {
        // Allocate scene_node on the stack for better performance
        ENTITY temp = ENTITY(id_generator.create_id(),l_type );
        if (l_parent != nullptr && l_parent->entity.type == DYNAMIC)temp.type = DYNAMIC;

        // Mark entry with temp entity
        Component_UNIT.markEntry(temp,shapeName,num_instnace);

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
    void Render_Scene() {


        //mere hussab se Ek hi pura shader switch insme hi rakhta hu or renderer mai ke unnessesary swich na ho
        
        
        glBindVertexArray(MERGED_VAO);
        glBindBufferBase(GL_SHADER_STORAGE_BLOCK, BINDIND_INDX_INST, MERGED_MODEL_SSBO);

        glBindBuffer(GL_DRAW_INDIRECT_BUFFER, MERGED_MDI_COMMAND);
        
        glMultiDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_SHORT, nullptr, NUM_MERGED_CMD, sizeof_mdi_commands);
    }


    void InitializeData() {
        //baadme isse public private dekhna abhi public maanke chal

        size_t TOTAL_VERTEX = Component_UNIT.next_MERGED_MDI_CMD.baseVertex;
        size_t TOTAL_INDEX = Component_UNIT.next_MERGED_MDI_CMD.firstIndex;

        std::cout << "TOTAL_VERTEX: " << TOTAL_VERTEX << std::endl;
        std::cout << "TOTAL_INDEX: " << TOTAL_INDEX << std::endl;



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

        for (auto& shape_shr : Component_UNIT.STORAGE.Shape_data) {

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
        //DEBUG
        if (vertices.empty())std::cout << "fuck verts\n";
        if (indices.empty())std::cout << "fuck inds\n";


        //binding the vAO before creation so i dont need to bind buffer again
        glBindVertexArray(MERGED_VAO);
        glCreateBuffers(1, &VBO);
        glNamedBufferData(VBO,TOTAL_VERTEX *sizeof(VERTEX), vertices.data(), GL_STATIC_DRAW); // OpenGL 4.5
        //linking not sure where this should be

        //ek baar hi set karnega as bining index 0 par ab kaam hoga with stride as vertexsize

        glVertexArrayVertexBuffer(MERGED_VAO, 0, VBO, 0, sizeof(VERTEX)); // OpenGL 4.5

        
        glEnableVertexArrayAttrib(MERGED_VAO, 0);
        glVertexArrayAttribFormat(MERGED_VAO, 0, 3, GL_FLOAT, GL_FALSE, offsetof(VERTEX, POS));
        glVertexArrayAttribBinding(MERGED_VAO, 0, 0);

        glEnableVertexArrayAttrib(MERGED_VAO, 1);
        glVertexArrayAttribFormat(MERGED_VAO, 1, 3, GL_FLOAT, GL_FALSE, offsetof(VERTEX, COLOR));
        glVertexArrayAttribBinding(MERGED_VAO, 1, 0);

        glEnableVertexArrayAttrib(MERGED_VAO, 2);
        glVertexArrayAttribFormat(MERGED_VAO, 2, 3, GL_FLOAT, GL_FALSE, offsetof(VERTEX, NORMAL));
        glVertexArrayAttribBinding(MERGED_VAO, 2, 0);

        glEnableVertexArrayAttrib(MERGED_VAO, 3);
        glVertexArrayAttribFormat(MERGED_VAO, 3, 2, GL_FLOAT, GL_FALSE, offsetof(VERTEX, TEX_COORDS));
        glVertexArrayAttribBinding(MERGED_VAO, 3, 0);

        //link Index buffer to VAO
        glCreateBuffers(1, &IBO);
        glNamedBufferData(IBO, TOTAL_INDEX * sizeof(unsigned short), indices.data(), GL_STATIC_DRAW);
        glVertexArrayElementBuffer(MERGED_VAO, IBO);
        /*
        
        FOR NOW I HAVE MAKE IT FIXED SIZE BEFORE COMPIALATION SIZE 
        IF THIS WROK MAKE IT RUNTIME SO TAKE BIGGGER SSBO SIZE 
        
        */
        glCreateBuffers(1, &MERGED_MODEL_SSBO);
        glNamedBufferStorage(MERGED_MODEL_SSBO, Component_UNIT.STORAGE.transform_data.size()*sizeof(glm::mat4), Component_UNIT.STORAGE.transform_data.data(), GL_DYNAMIC_STORAGE_BIT);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, BINDIND_INDX_INST, MERGED_MODEL_SSBO);
        //link Commandbuffer to VAO





        NUM_MERGED_CMD = Component_UNIT.STORAGE.indirect_commands_data.size();
        glGenBuffers(1, &MERGED_MDI_COMMAND);
        glBindBuffer(GL_DRAW_INDIRECT_BUFFER, MERGED_MDI_COMMAND);
        glBufferData(GL_DRAW_INDIRECT_BUFFER, sizeof_mdi_commands * NUM_MERGED_CMD ,
            Component_UNIT.STORAGE.indirect_commands_data.data(), GL_STATIC_DRAW);
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
