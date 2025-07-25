#include "Renderer.hpp"




//for debug model mat

//for running -temp



//problem ye aati thi ke func ke ander mai jo bhi obj banata tha ur uska reference kahinour ano.func me use karta tha tab tak destruct call hojata tha clearing that obj only pointer me adrress hota tha but not pointer 
//c/cpp me ye detect bhi nhi hota kyuki jo new address aata tha usse c typecast kardeta tha without checking the validity of that obj 
float Timer() {
	static double lastTime = 0.0;
	double currtime = glfwGetTime();
	float dt = float(currtime - lastTime);
	lastTime = currtime;
	return dt;
}
void checkError() {
	GLenum err = glGetError();
	if (err != GL_NO_ERROR) {
		std::cout << "RENDERER-SLIDER ERROR: " << err << "\n";
	}
}

// Callback function for keyboard input
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	static bool wire_frame = false;
	static bool mouseHidden = false;
	// Close window when ESC is pressed
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	if (key == GLFW_KEY_2 && action == GLFW_PRESS) {
		wire_frame = !wire_frame;
		glPolygonMode(GL_FRONT_AND_BACK, wire_frame ? GL_LINE : GL_FILL);
		std::cout << "Wireframe Mode: " << (wire_frame ? "ON\n" : "OFF\n") ;
	}
	if (key == GLFW_KEY_1 && action == GLFW_PRESS) {
		mouseHidden = !mouseHidden;
		glfwSetInputMode(window, GLFW_CURSOR, mouseHidden ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
		std::cout << "Mouse Cursor: " << (mouseHidden ? "HIDDEN\n" : "VISIBLE\n");
	}
}




void rend::Renderer::IMGUI_INIT(GLFWwindow* window_ptr)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;

	ImGui_ImplGlfw_InitForOpenGL(window_ptr, true);
	ImGui_ImplOpenGL3_Init("#version 460");
}

void rend::Renderer::IMGUI_RENDER(int fps)
{
	// Start the ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	// Create your UI here
	//idhar hamne ek window ka segment create kiyawith name this {if want to create more window seg. copy from this}
	ImGui::Begin("3D Renderer By Aman Gupta!");
	ImGui::Text("fps meter: %d",fps);
	ImGui::ColorEdit3("bg-Color", a_settings.background);
	ImGui::Text("Render distance:");
	ImGui::SliderInt("render", &a_settings.render_distance, 1, 10);
	ImGui::SliderInt("LOD", &a_settings.level_of_detail, 1, 10);


	ImGui::Text("DISPLAY");
	ImGui::Checkbox("ENABLE LIGHTS", &a_settings.env.light_enable);
	ImGui::SliderFloat("Ambient Light", &a_settings.env.ambient, 0.0f, 1.0f);
	
	int32_t selected_entity = a_scene->IMGUI_SCENE_PROPS();
	


	ImGui::End();

	a_aura.IMGUI_RENDER();

	// Render ImGui
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	UpdateModelmatrixInGPU(selected_entity);
}

void rend::Renderer::IMGUI_DESTROY(){
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}



void rend::Renderer::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	Renderer* renderer = static_cast<Renderer*>(glfwGetWindowUserPointer(window));
	if (renderer && renderer->a_editCam) {
		renderer->a_editCam->setScrollDelta(static_cast<float>(yoffset));
	}
}

void rend::Renderer::UpdateModelmatrixInGPU(int32_t changed_matrix_id)
{
	if (changed_matrix_id ==-1 ) return; // safety

	// Calculate offset
	GLsizeiptr offset = static_cast<GLsizeiptr>(changed_matrix_id) * sizeof(glm::mat4);
	glm::mat4 selected_matrix = a_component_UNIT->STORAGE.transform_data[changed_matrix_id].getModelMat();
	// Update specific matrix at index
	glNamedBufferSubData(MERGED_MODEL_SSBO, offset, sizeof(glm::mat4), &selected_matrix);
}

rend::Renderer::Renderer(LEVEL lvl,std::unique_ptr <WINDOW> ptr):a_win(std::move(ptr)) {
	//OPENGL -4.5 version with directStateAccess
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	a_win->construct();
	glfwMakeContextCurrent(a_win->ptr);
	//glfw se humne glad procedure address liya fir typecast kiya to glad provided script then load kiya into glad
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	//for 3d rendering


	//this func gives access to renderer class to glfw
	glfwSetWindowUserPointer(a_win->ptr, this);

	glViewport(0, 0, a_win->width, a_win->height);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);



	IMGUI_INIT(a_win->ptr);
	
	a_settings = CONFIG(lvl);
	a_aura = LightManager(3);

	a_shader = new Shader("Engine/Renderer/src/shaders/basic.vert", "Engine/Renderer/src/shaders/basic.frag");
	a_scene = new eng::SceneManager();
	//45 deg is ideal/ perfect for ORbit or editor style camera
	a_editCam = new eng::ArcBall(45.0f, 0.1f, 40.0f, (float(a_win->width) / (float)a_win->height));

	glCreateVertexArrays(1, &MERGED_VAO);

}

void rend::Renderer::UpdateBuffers(const std::shared_ptr<eng::ComponentManager>& Component_UNIT)
{
	//baadme isse public private dekhna abhi public maanke chal
	a_component_UNIT = Component_UNIT;
	
	size_t TOTAL_VERTEX = a_component_UNIT->next_MERGED_MDI_CMD.baseVertex;
	size_t TOTAL_INDEX = a_component_UNIT->next_MERGED_MDI_CMD.firstIndex;





	std::vector<VERTEX> vertices(TOTAL_VERTEX);
	// Ensure the vector has enough space, if base_vertex is calculated incorrectly
	if (vertices.size() < TOTAL_VERTEX) {
		vertices.resize(TOTAL_VERTEX);
	}

	std::vector<unsigned int>indices(TOTAL_INDEX);
	if (indices.size() < TOTAL_INDEX) {
		indices.resize(TOTAL_INDEX);
	}

	size_t offset_verts = 0;
	size_t vert_size_bytes;

	size_t offset_inds = 0;
	size_t inds_size_bytes;

	for (auto& shape_shr : a_component_UNIT->STORAGE.Shape_data) {

		vert_size_bytes = shape_shr->vertices.size() * sizeof(VERTEX);
		inds_size_bytes = shape_shr->indices.size() * sizeof(unsigned int);
		//if memeory error use std::copy
		memcpy(reinterpret_cast<char*>(vertices.data()) + offset_verts,
			shape_shr->vertices.data(), vert_size_bytes);
		memcpy(reinterpret_cast<char*>(indices.data()) + offset_inds,
			shape_shr->indices.data(), inds_size_bytes);

		offset_verts += vert_size_bytes;
		offset_inds += inds_size_bytes;

	}
	//DEBUG
	if (vertices.empty())std::cout << "fuck verts\n";
	if (indices.empty())std::cout << "fuck inds\n";
	//DEBUG
	std::cout << "RENDERER[DEBUG]-num_verts: "<<TOTAL_VERTEX<<"\n";
	std::cout << "RENDERER[DEBUG]-num_inds: " << TOTAL_INDEX << "\n";


	//binding the vAO before creation so i dont need to bind buffer again
	glBindVertexArray(MERGED_VAO);
	glCreateBuffers(1, &VBO);
	glNamedBufferData(VBO, TOTAL_VERTEX * sizeof(VERTEX), vertices.data(), GL_STATIC_DRAW); // OpenGL 4.5
	//linking not sure where this should be

	//ek baar hi set karnega as bining index 0 par ab kaam hoga with stride as vertexsize

	glVertexArrayVertexBuffer(MERGED_VAO, 0, VBO, 0, sizeof(VERTEX)); // OpenGL 4.5


	glEnableVertexArrayAttrib(MERGED_VAO, 0);
	glVertexArrayAttribFormat(MERGED_VAO, 0, 3, GL_FLOAT, GL_FALSE, offsetof(VERTEX, POS));
	glVertexArrayAttribBinding(MERGED_VAO, 0, 0);

	glEnableVertexArrayAttrib(MERGED_VAO, 1);
	glVertexArrayAttribFormat(MERGED_VAO, 1, 3, GL_FLOAT, GL_FALSE, offsetof(VERTEX, COLOR));
	glVertexArrayAttribBinding(MERGED_VAO, 1, 0);

	glEnableVertexArrayAttrib(MERGED_VAO, 2);
	glVertexArrayAttribFormat(MERGED_VAO, 2, 3, GL_FLOAT, GL_FALSE, offsetof(VERTEX, NORMAL));
	glVertexArrayAttribBinding(MERGED_VAO, 2, 0);

	glEnableVertexArrayAttrib(MERGED_VAO, 3);
	glVertexArrayAttribFormat(MERGED_VAO, 3, 2, GL_FLOAT, GL_FALSE, offsetof(VERTEX, TEX_COORDS));
	glVertexArrayAttribBinding(MERGED_VAO, 3, 0);

	//link Index buffer to VAO
	glCreateBuffers(1, &IBO);
	glNamedBufferData(IBO, TOTAL_INDEX * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
	glVertexArrayElementBuffer(MERGED_VAO, IBO);
	/*

	FOR NOW I HAVE MAKE IT FIXED SIZE BEFORE COMPIALATION SIZE
	IF THIS WROK MAKE IT RUNTIME SO TAKE BIGGGER SSBO SIZE

	*/
	glCreateBuffers(1, &MERGED_MODEL_SSBO);
	glNamedBufferStorage(MERGED_MODEL_SSBO,
		a_component_UNIT->STORAGE.matrix_data.size() * sizeof(glm::mat4),
		a_component_UNIT->STORAGE.matrix_data.data(),
		GL_DYNAMIC_STORAGE_BIT);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, BINDIND_INDX_INST, MERGED_MODEL_SSBO);
	//link Commandbuffer to VAO





	NUM_MERGED_CMD = a_component_UNIT->STORAGE.indirect_commands_data.size();
	std::cout << "RENDERER[DEBUG]-num_cmd : " << NUM_MERGED_CMD << "\n";
	glGenBuffers(1, &MERGED_MDI_COMMAND);
	glBindBuffer(GL_DRAW_INDIRECT_BUFFER, MERGED_MDI_COMMAND);
	glBufferData(GL_DRAW_INDIRECT_BUFFER,
		sizeof_mdi_commands * NUM_MERGED_CMD,
		a_component_UNIT->STORAGE.indirect_commands_data.data(),
		GL_STATIC_DRAW);
}

rend::Renderer::~Renderer()
{
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &IBO);
	glDeleteBuffers(1, &MERGED_MDI_COMMAND);
	glDeleteBuffers(1, &MERGED_MODEL_SSBO);
	glDeleteVertexArrays(1, &MERGED_VAO);
	IMGUI_DESTROY();
	glfwDestroyWindow(a_win->ptr);
	glfwTerminate();
}

void rend::Renderer::Run()
{
	float deltaTime;

	 

	//Terrain riverland;
	// 
	//keyboard event listener 
	glfwSetKeyCallback(a_win->ptr, keyCallback);
	glfwSetScrollCallback(a_win->ptr, rend::Renderer::scroll_callback);

	a_scene->loadModel("SWORD", "Resources/sword/scene.gltf");
	a_scene->loadModel("CAR", "Resources/demo/gtr-car.glb");
	a_scene->loadModel("CAR", "Resources/bunny/scene.gtf");

	a_scene->createEntity(STATIC,"SWORD");
	a_scene->createEntity(STATIC, "SPHERE", 0);


	UpdateBuffers(a_scene->getComponent2GPU());
	while (!glfwWindowShouldClose(a_win->ptr)) {
		deltaTime = Timer();
		a_shader->Activate();
		//std::cout << 1.0/deltaTime << "\n";

		a_settings.check4Change();
		glClearColor(a_settings.background[0], a_settings.background[1], a_settings.background[2], 1.0);
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//idhar camera move bhi horah hai fir uskA result ko update to gpu bhi kar raha hu
		a_editCam->Update(a_win->ptr);
		a_shader->UpdateCamPos(a_editCam->getCamPos());

		

		a_shader->viewMatrix(a_editCam->renderView());
		a_shader->projMatrix(a_editCam->getProjMatrix());
		a_shader->UpdateNUM_LIGHTS(a_aura.NUM_LIGHTS);


		a_scene->Update_Scene(deltaTime);
		//SCENE->Render_Scene();
		//mere hussab se Ek hi pura shader switch insme hi rakhta hu or renderer mai ke unnessesary swich na ho


		finalMDIRender();

		
		//riverland.dynamicLoad(cam, Aura.NUM_LIGHTS, settings.render_distance, settings.level_of_detail);
		IMGUI_RENDER(int(1/deltaTime));
		glfwSwapBuffers(a_win->ptr);

		glfwPollEvents();

		//SLIDER DEBUG METER
		if (DEBUGfirstrun)checkError();
		DEBUGfirstrun = false;
		//SLIDER DEBUG METER

	}
}

void rend::Renderer::finalMDIRender()
{
	glBindVertexArray(MERGED_VAO);
	

	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, BINDIND_INDX_INST, MERGED_MODEL_SSBO);

	glBindBuffer(GL_DRAW_INDIRECT_BUFFER, MERGED_MDI_COMMAND);

	glMultiDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_INT, nullptr, NUM_MERGED_CMD, sizeof_mdi_commands);
}


