#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/noise.hpp>
#include "terrain_vertex.h"
#include "../../Shape.h"
#include <memory>

//for dynamic vertex vector
#include <vector>


#define CHUNK_SIZE  8
#define INDEX_PER_FACE 6



float genHeight(float x, float z,const int& PERLIN_DETAIL) {
    float Height = 0.0;
    float Amplitude = 14.0;//INCREASE to MAKE more HEIGHT differnce in terrain
    float Frequency = 0.02;//DECREASE to MAKE FLATTER TERRAIN
    for (int i = 0; i < PERLIN_DETAIL; i++) {
        Height += glm::perlin(glm::vec2(x * Frequency, z * Frequency)) * Amplitude; // Scaled height
        Amplitude /= 8;
        Frequency *= 10;
    }
    return Height;
}

std::shared_ptr<Shape> generateChunk(const int chunk_x,const int chunk_z ,const int LEVEL_OF_DETAIL,const int PERLIN_DETAIL ,const float Seed) {

    std::shared_ptr<Shape> temp = std::make_shared<Shape>();
    //kyuki mene LOD introduce kiya toh numbers of vertex bhi dynamic hoga hence i need std::vector instead of array
    std::vector <terrain_VERTEX> vertices;
    const int MAX_VERTEX_PER_EDGE = (CHUNK_SIZE * LEVEL_OF_DETAIL) + 1;
    vertices.resize(MAX_VERTEX_PER_EDGE*MAX_VERTEX_PER_EDGE);
    int index = 0;

    const float vertSteps = 1 / (float)LEVEL_OF_DETAIL;
    for (int i = 0; i < MAX_VERTEX_PER_EDGE; ++i) {
        for (int j = 0; j < MAX_VERTEX_PER_EDGE; ++j) {
            float dx = (chunk_x * CHUNK_SIZE) + i * vertSteps ;
            float dz = (chunk_z * CHUNK_SIZE) + j * vertSteps ;
            vertices[index].POS = glm::vec3(dx, genHeight(dx + Seed, dz + Seed, PERLIN_DETAIL), dz);
            index++;
        }
    }
    std::vector <unsigned short>indices;
    indices.resize((MAX_VERTEX_PER_EDGE - 1)*(MAX_VERTEX_PER_EDGE - 1)*INDEX_PER_FACE);
    index = 0;
    for (int i = 0; i < MAX_VERTEX_PER_EDGE - 1; ++i) {
        for (int j = 0; j < MAX_VERTEX_PER_EDGE - 1; ++j) {

                int bot_left = i * MAX_VERTEX_PER_EDGE + j;
                int bot_right = bot_left + 1;
                int top_left = bot_left + MAX_VERTEX_PER_EDGE;
                int top_right = top_left + 1;

                indices[index++] = bot_left;  // A
                indices[index++] = bot_right; // C
                indices[index++] = top_left;  // B

                indices[index++] = top_left;  // B
                indices[index++] = bot_right; // C
                indices[index++] = top_right; // D
        }
    }
        glm::vec3 temp_vec_A, temp_vec_B;
        for (int i = 0; i < indices.size(); i+=3){
            //make a tri using index of indices 
            unsigned short p0 = indices[i];
            unsigned short p1 = indices[i+1];
            unsigned short p2 = indices[i+2];
            //make 2 vectors and cross pro gives normal ve of that area between two vector
            temp_vec_A = vertices[p1].POS - vertices[p0].POS;
            temp_vec_B = vertices[p2].POS - vertices[p0].POS;
            glm::vec3 temp_normal = glm::vec3(0.0);
            //a&b ko swap karke me normal ko negative kar sakta hu

            temp_normal = glm::cross(temp_vec_A, temp_vec_B);
            //add that normal vec to each vector
            vertices[p0].NORMAL += temp_normal;
            vertices[p1].NORMAL += temp_normal;
            vertices[p2].NORMAL += temp_normal;
        }

        for (int i = 0; i < vertices.size(); i++) {
            //make unit vector
            vertices[i].NORMAL = glm::normalize(vertices[i].NORMAL);
        }
        temp->LOD = LEVEL_OF_DETAIL;
        temp->PERLIN = PERLIN_DETAIL;
        temp->Update(vertices.data(), vertices.size(), indices.data(), indices.size());
        temp->send();
    return temp;
}
