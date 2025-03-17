#include "LightManager.h"

#include <glad/glad.h>

//
#include <../imgui/imgui.h>
#include <../imgui/imgui_impl_glfw.h>
#include <../imgui/imgui_impl_opengl3.h>

#include <string>
//DEBUG
#include <iostream>



void LightManager::DEB_printLight()
{
		for (int i = 0; i < NUM_LIGHTS; i++) {
			std::cout << "Light " << i << " -\nType: " << Lights[i].Type
				<< "\nPos: (" << Lights[i].Pos.x << ", " << Lights[i].Pos.y << ", " << Lights[i].Pos.z << ")\n"
				<< "Color: (" << Lights[i].Color.r << ", " << Lights[i].Color.g << ", " << Lights[i].Color.b << ")\n"
				<< "Intensity: " << Lights[i].Intensity << "\n";
		}
}

LightManager::LightManager()
{
}
//phele se hi bata de ki itna limit me kaam karna hai
LightManager::LightManager(int maxLIGHT):MAX_LIGHTS(maxLIGHT){
	glGenBuffers(1, &lightUBO);
	glBindBuffer(GL_UNIFORM_BUFFER, lightUBO);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(light) * MAX_LIGHTS, nullptr, GL_DYNAMIC_DRAW);
	glBindBufferRange(GL_UNIFORM_BUFFER, 1, lightUBO, 0, sizeof(light) * MAX_LIGHTS);
	glBindBufferBase(GL_UNIFORM_BUFFER, 1, lightUBO);

}

void LightManager::AddLight(const light& LIGHT)
{
	if (Lights.size() == MAX_LIGHTS)return ;
	Lights.push_back(LIGHT);
	
}

void LightManager::removeLight()
{
	if (Lights.size() <= 0)return ;
	Lights.pop_back();
}

void LightManager::UpdateLights()
{
	//DEB_printLight();
	NUM_LIGHTS = Lights.size();
	if (NUM_LIGHTS > 0) {
		glBindBuffer(GL_UNIFORM_BUFFER, lightUBO);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, Lights.size() * sizeof(light), Lights.data());
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}
	
}

void LightManager::IMGUI_RENDER()
{
	static int Selected_Object = 0; // Persistent selection

	ImGui::Begin("AURA: Dynamic Lights");

	if (ImGui::BeginCombo("Select Light", ("Light " + std::to_string(Selected_Object)).c_str()))
	{
		for (int i = 0; i < Lights.size(); ++i)
		{
			bool isSelected = (Selected_Object == i);
			if (ImGui::Selectable(("Light " + std::to_string(i)).c_str(), isSelected))
			{
				Selected_Object = i;
			}
			if (isSelected)
			{
				ImGui::SetItemDefaultFocus();
			}
		}
		// Add Light button (with proper disable handling)
		if (Lights.size() >= MAX_LIGHTS)
		{
			ImGui::BeginDisabled(true);
			ImGui::Button("Add Light"); // Show button, but disabled
			ImGui::EndDisabled();
		}
		else
		{
			if (ImGui::Button("Add Light"))
			{
				Lights.push_back(light());             // Add a new light
				Selected_Object = Lights.size() - 1;  // Select the newly added light
			}
		}


		ImGui::EndCombo();
	}

	if (!Lights.empty() && Selected_Object >= 0 && Selected_Object < Lights.size())
	{
		light& ob = Lights[Selected_Object]; // Direct reference to selected light

		ImGui::Text("Light %d", Selected_Object);
		ImGui::InputInt("Type:", &ob.Type);
		ImGui::DragFloat3("Pos/Dir:", &ob.Pos[0], 0.1f);
		ImGui::ColorEdit3("Color:", &ob.Color[0]);
		ImGui::SliderFloat("Intensity:", &ob.Intensity, 0.0f, 1.0f);
	}

	ImGui::End();

	UpdateLights();
}

