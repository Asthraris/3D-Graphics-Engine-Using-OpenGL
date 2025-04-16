#pragma once

#include <unordered_map>
#include <string>
#include <memory>  // For shared_ptr


#include "Shape.hpp"
#include "LoadModels.hpp"


constexpr double PI = 3.14159265358979323846;
constexpr double TWO_PI = 2.0 * PI;


namespace eng {

class ShapeLibrary {
	std::unordered_map<std::string, std::shared_ptr<Shape>> StoredShapes;
    std::unordered_map<std::string, glm::mat4> StoredMatrix;

    GLTFLoader LOADER;

public:
	ShapeLibrary() {
		//add default shapes
        createCube();
        createPlane();
        createPyramid();
        createSphere();
        createCylinder();
	}
    //dynamically loads model into shape library

    void addShapeData(const std::string& name, std::shared_ptr<Shape>shape) {
        StoredShapes[name] = shape;
    }
    void addShapeData(const std::string& name, const std::string& path ) {
        std::vector<MeshData> rawData;
        bool success = LOADER.loadGLTFobj(path, rawData);
        if (!success) {
            std::cerr << "Failed to load model at: " << path << std::endl;
            return;
        }

        if (rawData.empty()) {
            std::cerr << "No mesh data found in model: " << path << std::endl;
            return;
        }

        // Create shape and store mesh data
        std::shared_ptr<Shape> shapedata = std::make_shared<Shape>();
        shapedata->setMeshData(rawData[0].Positions, rawData[0].Normals, rawData[0].Indices);

        // Store shape and its transform matrix
        StoredShapes[name] = shapedata;

        StoredMatrix[name] = rawData[0].Transform_mat;
    }

    
    glm::mat4 getmatrixLoaded(const std::string& name) {
        auto it = StoredMatrix.find(name);
        if (it != StoredMatrix.end()) {
            return it->second;
        }
        return glm::mat4(1.0);
    }
	std::shared_ptr<Shape> getShapeData(const std::string& name) {
        auto it = StoredShapes.find(name);
        if (it != StoredShapes.end()) {
            return it->second;
        }
        return nullptr;  // Return `nullptr` if shape doesn't exist
	}
    //check creation code properly NEXT JOB
    void createCube(float size = 1.0f) {
        std::shared_ptr<Shape> cube = std::make_shared<Shape>();

        float half = size / 2.0f;

        cube->vertices = {
            // Front face
            {{-half, -half,  half}, {1, 1, 1}, {0, 0, 1}, {0, 0}},
            {{ half, -half,  half}, {1, 1, 1}, {0, 0, 1}, {1, 0}},
            {{ half,  half,  half}, {1, 1, 1}, {0, 0, 1}, {1, 1}},
            {{-half,  half,  half}, {1, 1, 1}, {0, 0, 1}, {0, 1}},

            // Back face
            {{-half, -half, -half}, {1, 1, 1}, {0, 0, -1}, {0, 0}},
            {{ half, -half, -half}, {1, 1, 1}, {0, 0, -1}, {1, 0}},
            {{ half,  half, -half}, {1, 1, 1}, {0, 0, -1}, {1, 1}},
            {{-half,  half, -half}, {1, 1, 1}, {0, 0, -1}, {0, 1}}
        };

        cube->indices = {
            0, 1, 2, 2, 3, 0,  // Front
            4, 5, 6, 6, 7, 4,  // Back
            0, 4, 7, 7, 3, 0,  // Left
            1, 5, 6, 6, 2, 1,  // Right
            3, 2, 6, 6, 7, 3,  // Top
            0, 1, 5, 5, 4, 0   // Bottom
        };

        addShapeData("CUBE", cube);
    }
    void createPlane(float size = 1.0f) {
        std::shared_ptr<Shape> plane = std::make_shared<Shape>();


        float half = size / 2.0f;

        plane->vertices = {
            {{-half, 0.0f, -half}, {1, 1, 1}, {0, 1, 0}, {0, 0}},
            {{ half, 0.0f, -half}, {1, 1, 1}, {0, 1, 0}, {1, 0}},
            {{ half, 0.0f,  half}, {1, 1, 1}, {0, 1, 0}, {1, 1}},
            {{-half, 0.0f,  half}, {1, 1, 1}, {0, 1, 0}, {0, 1}}
        };

        plane->indices = { 0, 1, 2, 2, 3, 0 };

        addShapeData("PLANE", plane);

    }
   
    void createPyramid(float size = 1.0f) {
        std::shared_ptr<Shape> pyramid = std::make_shared<Shape>();


        float half = size / 2.0f;

        pyramid->vertices = {
            {{0.0f, size, 0.0f}, {1, 1, 1}, {0, 1, 0}, {0.5f, 1.0f}},    // Tip
            {{-half, 0.0f, -half}, {1, 1, 1}, {0, -1, 0}, {0.0f, 0.0f}},  // Base 1
            {{ half, 0.0f, -half}, {1, 1, 1}, {0, -1, 0}, {1.0f, 0.0f}},  // Base 2
            {{ half, 0.0f,  half}, {1, 1, 1}, {0, -1, 0}, {1.0f, 1.0f}},  // Base 3
            {{-half, 0.0f,  half}, {1, 1, 1}, {0, -1, 0}, {0.0f, 1.0f}}   // Base 4
        };

        pyramid->indices = {
            0, 1, 2,    // Side 1
            0, 2, 3,    // Side 2
            0, 3, 4,    // Side 3
            0, 4, 1,    // Side 4
            1, 2, 3, 3, 4, 1   // Base
        };

        addShapeData("PYRAMID", pyramid);
    }
    void createSphere(float radius = 1.0f, int sectors = 24, int stacks = 16) {
        std::shared_ptr<Shape> sphere = std::make_shared<Shape>();

        float pi = 3.14159265359f;
        for (int i = 0; i <= stacks; ++i) {
            float lat = pi * (-0.5f + (float)i / stacks);
            float y = radius * sin(lat);
            float r = radius * cos(lat);

            for (int j = 0; j <= sectors; ++j) {
                float lon = 2.0f * pi * (float)j / sectors;
                float x = r * cos(lon);
                float z = r * sin(lon);

                glm::vec3 normal = glm::normalize(glm::vec3(x, y, z));
                sphere->vertices.emplace_back(glm::vec3(x, y, z), glm::vec3(1.0f), normal, glm::vec2((float)j / sectors, (float)i / stacks));

                if (i < stacks && j < sectors) {
                    int nextRow = (i + 1) * (sectors + 1);
                    int nextCol = j + 1;

                    sphere->indices.push_back(i * (sectors + 1) + j);
                    sphere->indices.push_back(nextRow + j);
                    sphere->indices.push_back(i * (sectors + 1) + nextCol);

                    sphere->indices.push_back(i * (sectors + 1) + nextCol);
                    sphere->indices.push_back(nextRow + j);
                    sphere->indices.push_back(nextRow + nextCol);
                }
            }
        }

        addShapeData("SPHERE", sphere);
    }
    void createCylinder(float radius = 1.0f, float height = 2.0f, int segments = 36) {
        std::shared_ptr<Shape> cylinder = std::make_shared<Shape>();

        float angleStep = 2.0f * 3.14159265359f / segments;
        float halfHeight = height / 2.0f;

        // Generate vertices for the side of the cylinder
        for (int i = 0; i <= segments; ++i) {
            float angle = i * angleStep;
            float x = radius * cos(angle);
            float z = radius * sin(angle);

            // Top and bottom circle vertices
            cylinder->vertices.emplace_back(glm::vec3(x, halfHeight, z), glm::vec3(1.0f), glm::vec3(x, 0, z), glm::vec2(i / (float)segments, 1.0f));
            cylinder->vertices.emplace_back(glm::vec3(x, -halfHeight, z), glm::vec3(1.0f), glm::vec3(x, 0, z), glm::vec2(i / (float)segments, 0.0f));
        }

        // Generate indices for sides
        for (int i = 0; i < segments; ++i) {
            int top1 = 2 * i;
            int bottom1 = top1 + 1;
            int top2 = 2 * ((i + 1) % segments);
            int bottom2 = top2 + 1;

            cylinder->indices.push_back(top1);
            cylinder->indices.push_back(bottom1);
            cylinder->indices.push_back(top2);

            cylinder->indices.push_back(top2);
            cylinder->indices.push_back(bottom1);
            cylinder->indices.push_back(bottom2);
        }

        // Generate center points for caps
        size_t topCenter = cylinder->vertices.size();
        cylinder->vertices.emplace_back(glm::vec3(0, halfHeight, 0), glm::vec3(1, 1, 1), glm::vec3(0, 1, 0), glm::vec2(0.5f, 0.5f));
        size_t bottomCenter = cylinder->vertices.size();
        cylinder->vertices.emplace_back(glm::vec3(0, -halfHeight, 0), glm::vec3(1, 1, 1), glm::vec3(0, -1, 0), glm::vec2(0.5f, 0.5f));

        // Generate indices for caps
        for (int i = 0; i < segments; ++i) {
            int next = (i + 1) % segments;
            cylinder->indices.push_back(topCenter);
            cylinder->indices.push_back(2 * next);
            cylinder->indices.push_back(2 * i);

            cylinder->indices.push_back(bottomCenter);
            cylinder->indices.push_back(2 * i + 1);
            cylinder->indices.push_back(2 * next + 1);
        }

        addShapeData("CYLINDER", cylinder);
    }



};
}