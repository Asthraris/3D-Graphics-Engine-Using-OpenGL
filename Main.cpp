#include "renderer/Renderer.h"

#include "renderer/Config.h"


#include <memory>
int main() {

	std::unique_ptr<WINDOW> win = std::make_unique<WINDOW>();
	win->width = 1080;
	win->height = 720;
	win->name = "HELLO";
	Renderer Sigma(godot,std::move(win));

	//LEVEL OF DETAIL USING vertexdensity distortion
	Sigma.Run();
	return 0;
}