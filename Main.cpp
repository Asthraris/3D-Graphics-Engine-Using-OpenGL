//standard libraries
#include <memory>


//Engine Spec Libs
#include "Engine/Renderer/Renderer.hpp"

#include "Engine/Renderer/Utils.hpp"


int main() {

	std::unique_ptr<WINDOW> win = std::make_unique<WINDOW>();
	win->width = 1080;
	win->height = 720;
	win->name = "HELLO";
	rend::Renderer myRenderer(godot,std::move(win));

	//LEVEL OF DETAIL USING vertexdensity distortion
	
	myRenderer.Run();
	return 0;
}





















//TODO_____>
//mujhe gpu to gpu changes only renderer me chaiye
//mene Shape.h ko alter kiya hai beacuse i need upgrading it for generatal shape not optinum for terrain now 
//so Terrain ke liye bhi karna hoga 
//abhi ke liye terrain work nahi hoga


//v2
/* ABHI ke liye me Shape data ko bhi include kar raha hu but baadme me shared ptr use karna chata hu to store similar shape point in one data Shape agar kuch naya shape hoga then i would provide an otion ko create an new Shape*/

//AGAR dynamically add karna hai toh mujhe rendererka initialize ko bhi baar baar call karna hoga with better update in buffer isliye abhi me sirf inatnces ko update karnunga