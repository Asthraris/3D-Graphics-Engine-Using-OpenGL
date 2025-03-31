#pragma once
#include <vector>
#include <deque>
#include <unordered_map>
#include <string>

#include "renderer/Transformation.h"
#include "Shape.h"
#include "Shape_Library.h"
enum entities_type{
	STATIC,
	DYNAMIC,
	INSTANCED,
	LIGHT,
	PARTICLES
};

struct ENTITY {
    uint32_t id;
    entities_type type;
    ENTITY(uint32_t l_id,entities_type l_type):id(l_id),type(l_type){}
};

//used for telling metadata for huge buffers to gpu instaed of calling multiple draw calls with offset
struct MDI_commands {
	size_t index_count;
	size_t instance_count;
	size_t base_index;
	size_t base_vertex;
	size_t base_instance;
};

struct comp_layout_renderer {
    size_t Trans_index;
    size_t shape_index;
    size_t commd_index;

};
struct entity_renderer_data {
    std::vector< Transformation> transform_map;
    std::vector< std::shared_ptr<Shape>> Shape_map;
    std::vector< MDI_commands> indirect_commands;
    std::unordered_map<uint32_t, comp_layout_renderer> Map_lookup;
};

//mai teeno components ka alag struct bana raha hu instead of having pure ECS
struct static_components: public entity_renderer_data {
	
};

struct dynamic_components:public entity_renderer_data {

};
// Special struct for instance entity transforms
struct instance_components: public entity_renderer_data {
                  
};

class ComponentManager {
private:
    //HOLDS the every Shape recored
    ShapeLibrary s_library;
    //holds next base loaction to directly assign them with comm_map of 
    MDI_commands next_static_cmd;
    MDI_commands next_dynamic_cmd;
    MDI_commands next_instance_cmd;

    //REAL DATA STORAGE BLOCKS
    static_components static_entities_data;
    dynamic_components dynamic_entities_data;
    instance_components instanced_entities_data;
public:
    //ye func jo enity factory se create hoga uske properties ko strore bas karega entity creation is hendeld by factory
    ComponentManager() {
        next_static_cmd = {0,1,0,0,0};
        next_dynamic_cmd = { 0,1,0,0,0 };
        //instance me hume instance count sppecify karna hoga
        next_instance_cmd = { 0,0,0,0,0 };

        s_library = ShapeLibrary();
    }


    void markEntry(ENTITY l_en ,std::string shapeName,const glm::mat4& l_model = glm::mat4(1.0), size_t num_inst = 1) {

        auto Storedsh = s_library.getShapeData(shapeName);
        size_t num_inds = Storedsh->indices.size();
        size_t num_verts = Storedsh->vertices.size();
        switch (l_en.type) {

        case STATIC:

            static_entities_data.Map_lookup[l_en.id] = {
                static_entities_data.transform_map.size(),
                static_entities_data.Shape_map.size(),
                static_entities_data.indirect_commands.size()
            };

            static_entities_data.Shape_map.push_back(Storedsh);
            static_entities_data.transform_map.emplace_back(Transformation(l_model));

            next_static_cmd = { num_inds, num_inst,
                           next_static_cmd.base_index + num_inds,
                           next_static_cmd.base_vertex + num_verts,
                           next_static_cmd.base_instance + num_inst };
            static_entities_data.indirect_commands.emplace_back(next_static_cmd);

            break;
        case DYNAMIC:

            dynamic_entities_data.Map_lookup[l_en.id] = {
            dynamic_entities_data.transform_map.size(),
            dynamic_entities_data.Shape_map.size(),
            dynamic_entities_data.indirect_commands.size()
            };

            dynamic_entities_data.Shape_map.push_back(Storedsh);
            dynamic_entities_data.transform_map.emplace_back(Transformation(l_model));

            next_dynamic_cmd = { num_inds, num_inst,
                                next_dynamic_cmd.base_index + num_inds,
                                next_dynamic_cmd.base_vertex + num_verts,
                                next_dynamic_cmd.base_instance + num_inst };

            dynamic_entities_data.indirect_commands.emplace_back(next_dynamic_cmd);
            break;

        case INSTANCED:
            instanced_entities_data.Map_lookup[l_en.id] = {
            instanced_entities_data.transform_map.size(),
            instanced_entities_data.Shape_map.size(),
            instanced_entities_data.indirect_commands.size()
            };

            instanced_entities_data.Shape_map.push_back(Storedsh);
            instanced_entities_data.transform_map.emplace_back(Transformation(l_model));

            next_instance_cmd = { num_inds, num_inst,
                                 next_instance_cmd.base_index + num_inds,
                                 next_instance_cmd.base_vertex + num_verts,
                                 next_instance_cmd.base_instance + num_inst };

            instanced_entities_data.indirect_commands.emplace_back(next_instance_cmd);
            break;
        default:
            return;
        }
    }

};

// ye enity create karega and their components ko bhi and strore karega in Comonents manger me 


class EntitiesIDGenerator {
private:
    uint32_t id_counter = 1;          // ID counter
    std::vector<uint32_t> free_ids;   //  Freed IDs for reuse

public:
    uint32_t create_id() {
        if (!free_ids.empty()) {
            uint32_t id = free_ids.back();    //  Reuse freed ID
            free_ids.pop_back();
            return id;
        }
        return id_counter++;
    }

    void destroy_id(uint32_t id) {
        free_ids.push_back(id);              //  Recycle ID
    }

    void reset() {
        free_ids.clear();
        id_counter = 1;
    }
};



