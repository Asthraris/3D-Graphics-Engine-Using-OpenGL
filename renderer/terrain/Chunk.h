#pragma once

#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/noise.hpp>
#include "../vertex.h"
#include "../../Shape.h"

#define CHUNK_SIZE  8
//MAX_VER_LINE = CHUNK_SIZE /VERTEX_DENSITY(1) +1
#define  MAX_VER_LINE  9
//NUM_VERTOCES = MAX_VER_LINE^2
#define  NUM_VERTICES  81
//NUM_indices = ((MAX_VER_LINE-1)^2) *6
#define  NUM_INDICES  384  


float genHeight(float x, float z,const int octaves) {
    float Height = 0.0;
    float Amplitude = 10.0;
    float Frequency = 0.05;
    for (int i = 0; i < octaves; i++) {
        Height += glm::perlin(glm::vec2(x * Frequency, z * Frequency)) * Amplitude; // Scaled height
        Amplitude /= 8;
        Frequency *= 10;
    }
    return Height;
}

std::shared_ptr<Shape> generateChunk(const int chunk_x,const int chunk_z) {
    int index = 0;
    std::shared_ptr<Shape> temp = std::make_shared<Shape>();
    VERTEX vertices[NUM_VERTICES];
    for (int i = 0; i < MAX_VER_LINE; ++i) {
        for (int j = 0; j < MAX_VER_LINE; ++j) {
            int dx = (chunk_x * CHUNK_SIZE) + i ;
            int dz = (chunk_z * CHUNK_SIZE) + j;
            vertices[index].POS = glm::vec3(dx, genHeight(dx, dz, 8), dz);
            index++;
        }
    }
        unsigned short indices[NUM_INDICES];
        index = 0;
        for (int i = 0; i < MAX_VER_LINE - 1; ++i) {
            for (int j = 0; j < MAX_VER_LINE - 1; ++j) {
                int bot_left = i * MAX_VER_LINE + j;
                int bot_right = bot_left + 1;
                int top_left = bot_left + MAX_VER_LINE;
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
        for (int i = 0; i < NUM_INDICES; i+=3){
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

        for (int i = 0; i < NUM_VERTICES; i++) {
            //make unit vector
            vertices[i].NORMAL = glm::normalize(vertices[i].NORMAL);
        }
        temp->Update(vertices, NUM_VERTICES, indices, NUM_INDICES);
        temp->send();
    return temp;
}
