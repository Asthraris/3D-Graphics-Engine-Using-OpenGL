#include "Terrain.h"

#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>



#include "Chunk.h"

#define RENDER_DIST 5

Terrain::Terrain():SHADER("renderer/src/shaders/terrain.vert", "renderer/src/shaders/terrain.frag") {
	
	MODEL_MATRIX = glm::mat4(1.0f);
	
}

Terrain::~Terrain()
{
	Map.clear();

}

void Terrain::dynamicLoad(float* view_mat, glm::vec2 Cam_Chunk_Loc){
	SHADER.Activate();
	SHADER.camMatrix(view_mat);
	SHADER.UpdateModelMatrix(glm::value_ptr(MODEL_MATRIX));

	for (int x = Cam_Chunk_Loc.x - RENDER_DIST; x < Cam_Chunk_Loc.x + RENDER_DIST; x++) {
		for (int y = Cam_Chunk_Loc.y - RENDER_DIST; y < Cam_Chunk_Loc.y + RENDER_DIST; y++) {
			
			if (Map[Grid(x, y)] == nullptr) {
			    Map[Grid(x, y)] = generateChunk(x, y);  
			}
			Map[Grid(x, y)]->render();
			
		}
	}
}

