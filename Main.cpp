#include "renderer/Renderer.h"

int main() {
	Renderer Sigma(720, 480, "SIGMA");
	Sigma.Init();
	Sigma.Run();
	return 0;
}