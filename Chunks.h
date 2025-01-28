#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/noise.hpp>
#include <vector>
#include <memory>
#include <glad/glad.h>

#include "Shape.h"

// Debug
#include <iostream>

#define CHUNK_SIZE 8
#define PERLIN_INTENSITY 0.3f
#define VERTEX_DENSITY 0.5f
#define PEAK_HEIGHT 2.0f

void debugTriangles(const std::vector<VERTEX>& vertices, const std::vector<unsigned short>& indices) {
    for (size_t i = 0; i < indices.size(); i += 3) {
        // Ensure indices are within valid range
        if (indices[i] >= vertices.size() || indices[i + 1] >= vertices.size() || indices[i + 2] >= vertices.size()) {
            std::cerr << "Error: Triangle " << i / 3 << " has an out-of-bounds index!" << std::endl;
            continue;
        }

        // Retrieve vertex positions
        glm::vec3 v1 = vertices[indices[i]].POS;
        glm::vec3 v2 = vertices[indices[i + 1]].POS;
        glm::vec3 v3 = vertices[indices[i + 2]].POS;

        // Print triangle info
        std::cout << "Triangle " << i / 3 << ": "
            << "V1 (" << v1.x << ", " << v1.y << ", " << v1.z << ") | "
            << "V2 (" << v2.x << ", " << v2.y << ", " << v2.z << ") | "
            << "V3 (" << v3.x << ", " << v3.y << ", " << v3.z << ")"
            << std::endl;

        // Check for degenerate triangles
        if ((v1.x == 0 && v1.y == 0 && v1.z == 0) ||
            (v2.x == 0 && v2.y == 0 && v2.z == 0) ||
            (v3.x == 0 && v3.y == 0 && v3.z == 0)) {
            std::cerr << "Warning: Triangle " << i / 3 << " contains (0,0,0)!" << std::endl;
        }
    }
}

std::shared_ptr<Shape> genChunk(const int chunk_loc_x, const int chunk_loc_y) {
    std::vector<VERTEX> vertices;
    std::vector<unsigned short> indices;

    // Vertex generation
    int tile_x = ceil(CHUNK_SIZE / VERTEX_DENSITY) + 1;
    int tile_z = ceil(CHUNK_SIZE / VERTEX_DENSITY) + 1;

    vertices.resize(tile_x * tile_z);
    float HEIGHT;
    int index = 0;

    for (int i = 0; i < tile_x; ++i) {
        for (int j = 0; j < tile_z; ++j) {
            float x = (chunk_loc_x * CHUNK_SIZE) + i * VERTEX_DENSITY;
            float z = (chunk_loc_y * CHUNK_SIZE) + j * VERTEX_DENSITY;
            HEIGHT = glm::perlin(glm::vec2(x * PERLIN_INTENSITY, z * PERLIN_INTENSITY)) * PEAK_HEIGHT; // Scaled height
            vertices[index++] = {
                glm::vec3(x, HEIGHT, z),
                glm::vec3(1.0f, 0.0f, 0.0f)
            };
        }
    }

    // Index generation
    indices.resize((tile_x - 1) * (tile_z - 1) * 6);
    index = 0;

    for (int i = 0; i < tile_x - 1; ++i) {
        for (int j = 0; j < tile_z - 1; ++j) {
            int bot_left = i * tile_z + j;
            int bot_right = bot_left + 1;
            int top_left = bot_left + tile_z;
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


    // Create and return the Shape
    std::shared_ptr<Shape> terr = std::make_shared<Shape>();
    terr->Update(vertices.data(), vertices.size(), indices.data(), indices.size());
    terr->send();

    return terr;
}
