#pragma once

#include <unordered_map>
#include <string>
#include <memory>  // For shared_ptr

#include "Shape.h"

constexpr double PI = 3.14159265358979323846;
constexpr double TWO_PI = 2.0 * PI;



class ShapeLibrary {
	std::unordered_map<std::string, std::shared_ptr<Shape>> StoredShapes;
public:
	ShapeLibrary() {
		//add default shapes
        createCube();
        createCylinder();
        createPlane();
        createPyramid();
        createSphere();
        
	}
	void addShapeData(const std::string& name, std::shared_ptr<Shape> shape) {
		StoredShapes[name] = shape;
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
            {{-half, -half,  half}, {1, 0, 0}, {0, 0, 1}, {0, 0}},
            {{ half, -half,  half}, {0, 1, 0}, {0, 0, 1}, {1, 0}},
            {{ half,  half,  half}, {0, 0, 1}, {0, 0, 1}, {1, 1}},
            {{-half,  half,  half}, {1, 1, 0}, {0, 0, 1}, {0, 1}},

            // Back face
            {{-half, -half, -half}, {1, 0, 1}, {0, 0, -1}, {0, 0}},
            {{ half, -half, -half}, {0, 1, 1}, {0, 0, -1}, {1, 0}},
            {{ half,  half, -half}, {1, 1, 1}, {0, 0, -1}, {1, 1}},
            {{-half,  half, -half}, {0.5, 0.5, 0.5}, {0, 0, -1}, {0, 1}}
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
            {{-half, 0.0f, -half}, {0.7f, 0.7f, 0.7f}, {0, 1, 0}, {0, 0}},
            {{ half, 0.0f, -half}, {0.7f, 0.7f, 0.7f}, {0, 1, 0}, {1, 0}},
            {{ half, 0.0f,  half}, {0.7f, 0.7f, 0.7f}, {0, 1, 0}, {1, 1}},
            {{-half, 0.0f,  half}, {0.7f, 0.7f, 0.7f}, {0, 1, 0}, {0, 1}}
        };

        plane->indices = { 0, 1, 2, 2, 3, 0 };

        addShapeData("PLANE", plane);

    }
    void createSphere(float radius = 1.0f, int sectors = 24, int stacks = 16) {
        std::shared_ptr<Shape> sphere = std::make_shared<Shape>();


        for (int i = 0; i <= stacks; ++i) {
            float lat = PI * (-0.5f + (float)i / stacks);
            float y = radius * sin(lat);
            float r = radius * cos(lat);

            for (int j = 0; j <= sectors; ++j) {
                float lon = 2 * PI * (float)j / sectors;
                float x = r * cos(lon);
                float z = r * sin(lon);

                sphere->vertices.push_back({
                    {x, y, z},
                    {x / radius, y / radius, z / radius},
                    {x, y, z},
                    {static_cast<float>(j) / sectors, static_cast<float>(i) / stacks}
                    });

                if (i < stacks && j < sectors) {
                    int nextRow = i + 1;
                    int nextCol = j + 1;

                    sphere->indices.push_back(i * (sectors + 1) + j);
                    sphere->indices.push_back(nextRow * (sectors + 1) + j);
                    sphere->indices.push_back(i * (sectors + 1) + nextCol);
                          
                    sphere->indices.push_back(i * (sectors + 1) + nextCol);
                    sphere->indices.push_back(nextRow * (sectors + 1) + j);
                    sphere->indices.push_back(nextRow * (sectors + 1) + nextCol);
                }
            }
        }

        addShapeData("SPHERE", sphere);
    }
    void createPyramid(float size = 1.0f) {
        std::shared_ptr<Shape> pyramid = std::make_shared<Shape>();


        float half = size / 2.0f;

        pyramid->vertices = {
            {{0.0f, size, 0.0f}, {1, 0, 0}, {0, 1, 0}, {0.5f, 1.0f}},    // Tip
            {{-half, 0.0f, -half}, {0, 1, 0}, {0, -1, 0}, {0.0f, 0.0f}},  // Base 1
            {{ half, 0.0f, -half}, {0, 0, 1}, {0, -1, 0}, {1.0f, 0.0f}},  // Base 2
            {{ half, 0.0f,  half}, {1, 1, 0}, {0, -1, 0}, {1.0f, 1.0f}},  // Base 3
            {{-half, 0.0f,  half}, {1, 0, 1}, {0, -1, 0}, {0.0f, 1.0f}}   // Base 4
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
    void createCylinder(float radius = 1.0f, float height = 2.0f, int segments = 36) {
        std::shared_ptr<Shape> cylinder = std::make_shared<Shape>();


        float angleStep = 2 * PI / segments;

        for (int i = 0; i <= segments; ++i) {
            float angle = i * angleStep;
            float x = radius * cos(angle);
            float z = radius * sin(angle);

            cylinder->vertices.push_back({ {x, height / 2, z}, {1, 0, 0}, {x, 0, z}, {0, 0} });
            cylinder->vertices.push_back({ {x, -height / 2, z}, {0, 0, 1}, {x, 0, z}, {1, 0} });
        }

        addShapeData("CYLINDER", cylinder);
    }
};