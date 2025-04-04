#include "renderer/Renderer.h"

#include "renderer/Config.h"


#include <memory>
int main() {

	std::unique_ptr<WINDOW> win = std::make_unique<WINDOW>();
	win->width = 1920;
	win->height = 1080;
	win->name = "HELLO";
	Renderer Sigma(godot,std::move(win));

	//LEVEL OF DETAIL USING vertexdensity distortion
	Sigma.Run();
	return 0;
}


//mene Shape.h ko alter kiya hai beacuse i need upgrading it for generatal shape not optinum for terrain now 
//so Terrain ke liye bhi karna hoga 
//abhi ke liye terrain work nahi hoga


//v2
/* ABHI ke liye me Shape data ko bhi include kar raha hu but baadme me shared ptr use karna chata hu to store similar shape point in one data Shape agar kuch naya shape hoga then i would provide an otion ko create an new Shape*/