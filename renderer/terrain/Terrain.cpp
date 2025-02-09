#include "Terrain.h"

#include <cmath>


#define NUM_INDICES 1536
std::shared_ptr<Chunk_Render> genRenderSlot(unsigned int ibo) {
    std::shared_ptr<Chunk_Render> temp = std::make_shared<Chunk_Render>(ibo);
    return temp;
}

Terrain::Terrain(int render_distance):
	RENDER_DIST(render_distance)
{
    glGenBuffers(1, &IBO);
    {
        //made General Indices than link to all VAO so
        unsigned short indices[NUM_INDICES];
        int index = 0;
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
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, NUM_INDICES*sizeof(unsigned short), indices, GL_STATIC_DRAW);
    }


    //generating rendererin slots for render
    for (int x = render_distance; x <= render_distance; x++){
        for (int z = -render_distance; z <= render_distance; z++){
            Slots[Grid(x, z)] = genRenderSlot(IBO);
        }
    }

}

Terrain::~Terrain()
{
    glDeleteBuffers(1, &IBO);
}

void Terrain::DynamicLoad(glm::vec2 Cam_Chunk_Loc)
{
	for (int x = Cam_Chunk_Loc.x - RENDER_DIST; x < Cam_Chunk_Loc.x + RENDER_DIST; x++) {
		for (int y = Cam_Chunk_Loc.y - RENDER_DIST; y < Cam_Chunk_Loc.y + RENDER_DIST; y++) {
			if (Map[Grid(x, y)] == nullptr) {
                Map[Grid(x, y)] = genChunk(x, y);
                
			}
            Slots[Grid(x, y)]->Update(Map[Grid(x, y)]->vertices);
		}
	}
}
