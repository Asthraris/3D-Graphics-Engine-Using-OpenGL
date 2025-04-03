#pragma once
#include <vector>
#include <deque>
#include <map>
#include <string>

#include "Shape.h"
#include "Shape_Library.h"
enum entities_type{
	STATIC,
	DYNAMIC,
	INSTANCED,
	LIGHT,
	PARTICLES
};//type wala system ko mene drop kardiya balki ab me persistant map use karunga for instnace sso no need to make dynamic buffer,aur vese bhu vertex and index data kaha update hota hai {baad me usse bhi dynamic kar dunga for addition of entity on runtime}

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

struct MERGED_entity_renderer_data {
    std::vector<glm::mat4> transform_data;
    std::vector< std::shared_ptr<Shape>> Shape_data;
    std::vector< MDI_commands> indirect_commands_data;
};



class ComponentManager {
private:
    //HOLDS the every Shape recored
    ShapeLibrary s_library;

public:

    //holds next base loaction to directly assign them with comm_map of 
    MDI_commands next_MERGED_MDI_CMD;
    
    //REAL DATA STORAGE BLOCKS
    MERGED_entity_renderer_data STORAGE;
    //ye func jo enity factory se create hoga uske properties ko strore bas karega entity creation is hendeld by factory

    ComponentManager() {
        
        //instance me hume instance count sppecify karna hoga
        next_MERGED_MDI_CMD = { 0,0,0,0,0 };

        s_library = ShapeLibrary();
    }


    void markEntry(ENTITY l_en, std::string shapeName, size_t num_inst = 1, const glm::mat4& l_model = glm::mat4(1.0)) {

        auto Storedsh = s_library.getShapeData(shapeName);
        size_t num_inds = Storedsh->indices.size();
        size_t num_verts = Storedsh->vertices.size();

        // Compute offsets BEFORE updating the map
        MDI_commands currentCmd = {
            num_inds, num_inst,
            next_MERGED_MDI_CMD.base_index,    // Use existing offset
            next_MERGED_MDI_CMD.base_vertex,
            next_MERGED_MDI_CMD.base_instance
        };


        STORAGE.indirect_commands_data[l_en.id] = currentCmd;

        STORAGE.Shape_data.push_back(Storedsh);
        for (size_t i = 0; i < num_inst; i++)
        {
            STORAGE.transform_data.emplace_back(l_model);
        }

        // Update offsets for next entry
        next_MERGED_MDI_CMD.base_index += num_inds;
        next_MERGED_MDI_CMD.base_vertex += num_verts;
        next_MERGED_MDI_CMD.base_instance += num_inst;

    }
};



// ye enity create karega and their components ko bhi and strore karega in Comonents manger me 
class EntitiesIDGenerator {
private:
    uint32_t id_counter = 1;          // ID counter
    
public:
    uint32_t create_id() {
        return id_counter++;
    }

    void destroy_id(uint32_t id) {
                      //  Recycle ID
    }

    void reset() {
        id_counter = 1;
    }
};



