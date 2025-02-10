#pragma once

#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/noise.hpp>
#include "../vertex.h"
#include "../../Shape.h"

#define CHUNK_SIZE  8
#define VERTEX_DENSITY  0.5
#define  TOTAL_MINISQ  17
#define  NUM_VERTICES  289
#define  NUM_INDICES  1536

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

std::shared_ptr<Shape> generateChunk(const int chunk_x,const int chunk_y) {
    float HEIGHT;
    glm::vec3 COLOR;
    int index = 0;
    std::shared_ptr<Shape> temp = std::make_shared<Shape>();
    VERTEX vertices[1536];
    for (int i = 0; i < TOTAL_MINISQ; ++i) {
        for (int j = 0; j < TOTAL_MINISQ; ++j) {
            float dx = (chunk_x * CHUNK_SIZE) + i * VERTEX_DENSITY;
            float dz = (chunk_y * CHUNK_SIZE) + j * VERTEX_DENSITY;
            HEIGHT = genHeight(dx, dz,8);

            if (HEIGHT < 0.0) {
                COLOR = glm::vec3(0.0f, 0.0f, 1.0f + HEIGHT / 8);
            }
            else if (HEIGHT > 9.0) {
                COLOR = glm::vec3(HEIGHT / 4, HEIGHT / 8, HEIGHT / 8);
            }
            else {
                COLOR = glm::vec3(0.0, 0.2 > (HEIGHT / 8) ? 0.2 : HEIGHT / 8, 0.0f);
            }
            vertices[index++] = {glm::vec3(dx,HEIGHT , dz),COLOR};
        }
    }
        unsigned short indices[NUM_INDICES];
        index = 0;
        for (int i = 0; i < TOTAL_MINISQ - 1; ++i) {
            for (int j = 0; j < TOTAL_MINISQ - 1; ++j) {
                int bot_left = i * TOTAL_MINISQ + j;
                int bot_right = bot_left + 1;
                int top_left = bot_left + TOTAL_MINISQ;
                int top_right = top_left + 1;

                // First triangle
                indices[index++] = bot_left;
                indices[index++] = top_left;
                indices[index++] = bot_right;

                // Second triangle
                indices[index++] = bot_right;
                indices[index++] = top_left;
                indices[index++] = top_right;
            }
        }
        temp->Update(vertices, NUM_VERTICES, indices, NUM_INDICES);
        temp->send();
    return temp;
}
