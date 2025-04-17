#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <unordered_map>
//DEBUG
#include <iostream>

//user
#include "Transformation.hpp"
#include "Entities.hpp"
#include "Shape_Library.hpp"
#include "Shape.hpp"



//used for telling metadata for huge buffers to gpu instaed of calling multiple draw calls with offset
struct DrawElementsIndirectCommand {
    GLuint count;         // Number of indices
    GLuint instanceCount; // Number of instances
    GLuint firstIndex;    // Offset into the element array buffer
    GLint baseVertex;     // Offset to the first vertex
    GLuint baseInstance;  // REQUIRED for gl_BaseInstance!
};

struct MERGED_entity_renderer_data {
    std::vector<eng::Transformation> transform_data;
    std::vector<glm::mat4> matrix_data ;
    std::vector< std::shared_ptr<Shape>> Shape_data;
    std::vector< DrawElementsIndirectCommand> indirect_commands_data;
};
struct props_Locator {
    uint32_t id;
    uint32_t instance_index;
    props_Locator(uint32_t l_id , uint32_t l_ist=0):id(l_id),instance_index(l_ist){}
};
struct DYNAMIC_entity_props {

};


namespace eng {

    class ComponentManager {
    private:
    
    public:
    
        //HOLDS the every Shape recored
        ShapeLibrary s_library;
        //holds next base loaction to directly assign them with comm_map of 
        DrawElementsIndirectCommand next_MERGED_MDI_CMD;
    
        //REAL DATA STORAGE BLOCKS
        MERGED_entity_renderer_data STORAGE;
        //ye func jo enity factory se create hoga uske properties ko strore bas karega entity   creation is hendeld by factory

        //std::unordered_map < props_Locator, DYNAMIC_entity_props> PROPS_map;

        void IMGUI_COMP_PROPS(int32_t curr_id) {
            if (curr_id == -1)return;
            ImGui::BeginChild("TRANSFORM PROS", ImVec2(0, 150), true);
            //yaha pat temp instance bananya sayad se sahi hoga
            ImGui::Text("TRANSFORM MANAGER");
            ImGui::DragFloat3("Position: ", &STORAGE.transform_data[curr_id].Position.x);
            ImGui::DragFloat3("Scale: ", &STORAGE.transform_data[curr_id].Scale.x);
            ImGui::SliderFloat("Degrees rotation: ", &STORAGE.transform_data[curr_id].Rotation_degree, 0, 360.0f);
            ImGui::DragFloat3("Rotational_axis: ", &STORAGE.transform_data[curr_id].Rotational_axis.x);
            


            ImGui::EndChild();
        }
    
        ComponentManager() {
    
            //instance me hume instance count sppecify karna hoga
            next_MERGED_MDI_CMD = { 0,0,0,0,0 };
    
            s_library = ShapeLibrary();
        }
        void DEBUG_printCommand(const DrawElementsIndirectCommand cmd) {
            std::cout << "Count: " << cmd.count
                << ", Instances: " << cmd.instanceCount
                << ", First Index: " << cmd.firstIndex
                << ", Base Vertex: " << cmd.baseVertex
                << ", Base Instance: " << cmd.baseInstance << std::endl;
        }
    
        void markEntry(ENTITY l_en, std::string shapeName_or_path, size_t num_inst = 1) {
    
            auto Storedsh = s_library.getShapeData(shapeName_or_path);
            if (Storedsh == nullptr)std::cout << "Shape pointer not fetched from loaded shapes\n";
            size_t num_inds = Storedsh->indices.size();
            size_t num_verts = Storedsh->vertices.size();
    
            // Compute offsets BEFORE updating the map
            DrawElementsIndirectCommand currentCmd = {
                num_inds, num_inst,
                next_MERGED_MDI_CMD.firstIndex,    // Use existing offset
                next_MERGED_MDI_CMD.baseVertex,
                next_MERGED_MDI_CMD.baseInstance
            };
            //DEBUG_printCommand(currentCmd);
    
            STORAGE.indirect_commands_data.emplace_back(currentCmd);
    
            STORAGE.Shape_data.push_back(Storedsh);
            for (size_t i = 0; i < num_inst; i++)
            {
                STORAGE.transform_data.emplace_back(Transformation(s_library.getmatrixLoaded(shapeName_or_path)));
                
                STORAGE.matrix_data.emplace_back(
                    STORAGE.transform_data.back().getModelMat()
                );
                STORAGE.transform_data.back().mat_ptr = &STORAGE.matrix_data.back();
            }
    
            // Update offsets for next entry
            next_MERGED_MDI_CMD.firstIndex += num_inds;
            next_MERGED_MDI_CMD.baseVertex += num_verts;
            next_MERGED_MDI_CMD.baseInstance += num_inst;
    
        }
    };
}