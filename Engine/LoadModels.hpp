#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "GLTF/tiny_gltf.h"
#include "Transformation.hpp"

struct MeshData {
    std::vector<glm::vec3> Positions;
    std::vector<glm::vec3> Normals;
    std::vector<unsigned short> Indices;
    glm::mat4 Transform_mat;
};




class GLTFLoader {
public:
    void PrintMat4(const glm::mat4& mat) {
        for (int row = 0; row < 4; ++row) {
            std::cout << "| ";
            for (int col = 0; col < 4; ++col) {
                std::cout << mat[col][row] << " ";
            }
            std::cout << "|\n";
        }
    }
    bool ends_with(const std::string& str, const std::string& suffix) {
        return str.size() >= suffix.size() &&
            str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
    }

    void extractMeshData(const tinygltf::Model& model, const tinygltf::Node& node, const glm::mat4& parentTrans, std::vector<MeshData>& outmeshes) {
        std::cout << "[Debug] Extracting mesh data from node...\n";

        eng::Transformation local_trans;
        glm::mat4 localmat = glm::mat4(1.0f);
        if (!node.matrix.empty()) {
            std::cout << "[Debug] Node has matrix.\n";
            localmat = glm::make_mat4x4(node.matrix.data());
        }
        else {
            std::cout << "[Debug] Node missing matrix. Using default.\n";
            localmat = local_trans.getModelMat();
        }
        local_trans.mat_ptr = &localmat;

        glm::mat4 globalmat = parentTrans * localmat;

        if (node.mesh >= 0) {
            std::cout << "[Debug] Node references mesh index: " << node.mesh << "\n";
            const auto& mesh = model.meshes[node.mesh];

            for (const auto& primitive : mesh.primitives) {
                MeshData data;
                data.Transform_mat = globalmat;

                // Extract positions
                auto posAttr = primitive.attributes.find("POSITION");
                if (posAttr == primitive.attributes.end()) {
                    std::cerr << "[Error] POSITION attribute missing in primitive.\n";
                    continue;
                }
                const auto& posAccessor = model.accessors[posAttr->second];
                const auto& posView = model.bufferViews[posAccessor.bufferView];
                const auto& posBuffer = model.buffers[posView.buffer];

                std::cout << "[Debug] Position count: " << posAccessor.count << "\n";
                const float* positions = reinterpret_cast<const float*>(&posBuffer.data[posView.byteOffset + posAccessor.byteOffset]);
                for (size_t i = 0; i < posAccessor.count; ++i)
                    data.Positions.emplace_back(positions[i * 3 + 0], positions[i * 3 + 1], positions[i * 3 + 2]);

                // Extract normals
                auto normAttr = primitive.attributes.find("NORMAL");
                if (normAttr != primitive.attributes.end()) {
                    const auto& normAccessor = model.accessors[normAttr->second];
                    const auto& normView = model.bufferViews[normAccessor.bufferView];
                    const auto& normBuffer = model.buffers[normView.buffer];
                    const float* normals = reinterpret_cast<const float*>(&normBuffer.data[normView.byteOffset + normAccessor.byteOffset]);
                    for (size_t i = 0; i < normAccessor.count; ++i)
                        data.Normals.emplace_back(normals[i * 3 + 0], normals[i * 3 + 1], normals[i * 3 + 2]);
                }
                else {
                    std::cout << "[Warning] No NORMAL data found.\n";
                }

                // Extract indices (with component type check)
                if (primitive.indices >= 0) {
                    const auto& idxAccessor = model.accessors[primitive.indices];
                    const auto& idxView = model.bufferViews[idxAccessor.bufferView];
                    const auto& idxBuffer = model.buffers[idxView.buffer];


                    switch (idxAccessor.componentType) {
                    case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE: {
                        const uint8_t* indices = reinterpret_cast<const uint8_t*>(&idxBuffer.data[idxView.byteOffset + idxAccessor.byteOffset]);
                        for (size_t i = 0; i < idxAccessor.count; ++i)
                            data.Indices.push_back(static_cast<unsigned short>(indices[i]));
                        break;
                    }
                    case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT: {
                        const uint16_t* indices = reinterpret_cast<const uint16_t*>(&idxBuffer.data[idxView.byteOffset + idxAccessor.byteOffset]);
                        for (size_t i = 0; i < idxAccessor.count; ++i)
                            data.Indices.push_back(indices[i]);
                        break;
                    }
                    case TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT: {
                        const uint32_t* indices = reinterpret_cast<const uint32_t*>(&idxBuffer.data[idxView.byteOffset + idxAccessor.byteOffset]);
                        for (size_t i = 0; i < idxAccessor.count; ++i)
                            data.Indices.push_back(static_cast<unsigned short>(indices[i])); // Truncation: This can lead to overflow if index > 65535
                        break;
                    }
                    default:
                        std::cerr << "[Error] Unsupported index component type: " << idxAccessor.componentType << "\n";
                        break;
                    }
                }
                else {
                    std::cout << "[Warning] No index buffer found.\n";
                }
                outmeshes.push_back(std::move(data));
            }
        }

        // Recurse for child nodes
        for (const int child : node.children) {
            std::cout << "[Debug] Recursing child node: " << child << "\n";
            extractMeshData(model, model.nodes[child], globalmat, outmeshes);
        }
    }

    bool loadGLTFobj(const std::string& path, std::vector<MeshData>& meshes) {
        tinygltf::Model model;
        tinygltf::TinyGLTF loader;
        std::string err, warn;

        bool ret=false;
        if (ends_with(path, ".glb")) {
            std::cout << "[Debug] Loading GLB file: " << path << "\n";
            ret =loader.LoadBinaryFromFile(&model, &err, &warn, path);
        }
        else {
            std::cout << "[Debug] Loading GLTF file: " << path << "\n";
            ret =loader.LoadASCIIFromFile(&model, &err, &warn, path);
        }
        if (!warn.empty()) std::cout << "[GLTF-WARN] " << warn << "\n";
        if (!err.empty()) std::cout << "[GLTF-ERROR] " << err << "\n";
        if (!ret) {
            std::cerr << "[Fatal] Failed to load GLTF.\n";
            return false;
        }

        // Extract meshes from the default scene
        for (const auto& nodeIdx : model.scenes[model.defaultScene].nodes) {
            std::cout << "[Debug] Root node index: " << nodeIdx << "\n";
            extractMeshData(model, model.nodes[nodeIdx], glm::mat4(1.0f), meshes);
        }

        std::cout << "[Success] Total meshes extracted: " << meshes.size() << "\n";
        return true;
    }
};
