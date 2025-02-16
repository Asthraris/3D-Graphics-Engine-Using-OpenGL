#include "renderer/Renderer.h"

int main() {
	Renderer Sigma(1080, 720, "SIGMA");

	//LEVEL OF DETAIL USING vertexdensity distortion
	Sigma.Run();
	return 0;
}