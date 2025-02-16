#include "Terrain.h"

#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>



#include "Chunk.h"

#define RENDER_DIST 5

Terrain::Terrain()
{
	Model_Matrix = glm::mat4(1.0f);
}

Terrain::~Terrain()
{
	Map.clear();

}

void Terrain::dynamicLoad(glm::vec2 Cam_Chunk_Loc){
	
	for (int x = Cam_Chunk_Loc.x - RENDER_DIST; x < Cam_Chunk_Loc.x + RENDER_DIST; x++) {
		for (int y = Cam_Chunk_Loc.y - RENDER_DIST; y < Cam_Chunk_Loc.y + RENDER_DIST; y++) {
			
			if (Map[Grid(x, y)] == nullptr) {
			    Map[Grid(x, y)] = generateChunk(x, y);  
			}
			Map[Grid(x, y)]->render();
			
		}
	}
}

float* Terrain::getModelMatrix()
{
	return glm::value_ptr(Model_Matrix);
}
