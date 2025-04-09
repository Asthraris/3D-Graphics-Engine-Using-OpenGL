#pragma once
#include <vector>
#include <deque>
#include <map>
#include <string>
#include <iostream>


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

namespace eng {

// ye enity create karega and their components ko bhi and strore karega in Comonents manger me 
class EntitiesIDGenerator {
private:
    uint32_t id_counter = 0;          // ID counter
    
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

}


