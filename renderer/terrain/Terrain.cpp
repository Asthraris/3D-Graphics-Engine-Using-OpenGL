#include "Terrain.h"

#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>



#include "Chunk.h"
#include "../RandomGenerator.h"


//DEBUG
//#include <glad/glad.h>
//#include <iostream>




Terrain::Terrain(){
	SHADER =  new Shader("renderer/src/shaders/terrain.vert", "renderer/src/shaders/terrain.frag");

	RandomGenerator rg;
	SEED = rg.getFloat(-1000.0,1000.0);

}

Terrain::Terrain(float seed):SEED(seed){
	SHADER = new Shader("renderer/src/shaders/terrain.vert", "renderer/src/shaders/terrain.frag");
	
}

Terrain::~Terrain()
{
	Map.clear();
}

void Terrain::dynamicLoad(Camera& cam, const int& NUM_LIGHTS, const int& RenderDistance ,const int& TERR_LOD){

	SHADER->Activate();
	SHADER->viewMatrix(cam.renderView());
	SHADER->UpdateModelMatrix(trans->getMat());
	SHADER->projMatrix(cam.getProjMatrix());
	SHADER->UpdateNUM_LIGHTS(NUM_LIGHTS);
	glm::vec2 Curr_Chunk = cam.giveCamChunk();

	for (int x = Curr_Chunk.x -  RenderDistance; x < Curr_Chunk.x + RenderDistance; x++) {
		for (int y = Curr_Chunk.y - RenderDistance; y < Curr_Chunk.y + RenderDistance; y++) {
			 //filhaal mene shaoe ko naya general banaya hai hence isko fix karna hoga later
			if (Map[Grid(x, y)] == nullptr || Map[Grid(x, y)]->LOD !=TERR_LOD ) {
			    Map[Grid(x, y)] = generateChunk(x, y,TERR_LOD,BUBBLES , SEED);  
			}
			Map[Grid(x, y)]->render();
			
			
		}
	}
}

