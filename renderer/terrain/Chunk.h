#pragma once

#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/noise.hpp>
#include "../vertex.h"

const int CHUNK_SIZE = 8;
const float VERTEX_DENSITY = 0.5;
const int TOTAL_MINISQ = ceil(CHUNK_SIZE / VERTEX_DENSITY) + 1;
const int NUM_VERTICES = TOTAL_MINISQ * TOTAL_MINISQ;
const int NUM_INDICES = (TOTAL_MINISQ - 1) * (TOTAL_MINISQ - 1) * 6;

struct HeightMap{
	VERTEX vertices[1536];
};


float genHeight(float x, float z,const int octaves) {
    float Height = 0.0;
    float Amplitude = 10.0,
        Frequency = 0.05;
    for (int i = 0; i < octaves; i++) {
        Height += glm::perlin(glm::vec2(x * Frequency, z * Frequency)) * Amplitude; // Scaled height
        Amplitude /= 8;
        Frequency *= 10;
    }
    return Height;
}

std::shared_ptr<HeightMap> genChunk(int x,int y) {
    float HEIGHT;
    glm::vec3 COLOR;
    int index = 0;
    std::shared_ptr<HeightMap> temp = std::make_shared<HeightMap>();
    for (int i = 0; i < TOTAL_MINISQ; ++i) {
        for (int j = 0; j < TOTAL_MINISQ; ++j) {
            float dx = (x * CHUNK_SIZE) + i * VERTEX_DENSITY;
            float dz = (y * CHUNK_SIZE) + j * VERTEX_DENSITY;
            HEIGHT = genHeight(dx, dz, 8);

            if (HEIGHT < 0.0) {
                COLOR = glm::vec3(0.0f, 0.0f, 1.0f + HEIGHT / 8);
            }
            else if (HEIGHT > 9.0) {
                COLOR = glm::vec3(HEIGHT / 4, HEIGHT / 8, HEIGHT / 8);
            }
            else {
                COLOR = glm::vec3(0.0, 0.2 > (HEIGHT / 8) ? 0.2 : HEIGHT / 8, 0.0f);
            }
            temp->vertices[index++] = {glm::vec3(dx,HEIGHT , dz),COLOR};
        }
    }
    return temp;
}
