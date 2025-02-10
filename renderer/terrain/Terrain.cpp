#include "Terrain.h"

#include <cmath>

#include "Chunk.h"

#define RENDER_DIST 2

Terrain::~Terrain()
{
	Map.clear();

}

void Terrain::dynamicLoad(glm::vec2 Cam_Chunk_Loc)
{
	
	for (int x = Cam_Chunk_Loc.x - RENDER_DIST; x < Cam_Chunk_Loc.x + RENDER_DIST; x++) {
		for (int y = Cam_Chunk_Loc.y - RENDER_DIST; y < Cam_Chunk_Loc.y + RENDER_DIST; y++) {
			if (Map[Grid(x, y)] == nullptr) {
                Map[Grid(x, y)] = generateChunk(x, y);  
            }
            else {
                Map[Grid(x, y)]->render();
            }
            
		}
	}
}
