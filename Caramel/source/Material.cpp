#include "Material.h"
#include "glm/gtc/type_ptr.hpp"
#include "imgui.h"

Material::Material()
{

}

Material::~Material()
{

}

void Material::OnGUI()
{
	bool buttons[4];
	ImGui::RadioButton("Opaque", &buttons[0]);
	ImGui::RadioButton("Cutout", &buttons[1]);
	ImGui::RadioButton("Fade", &buttons[2]);
	ImGui::RadioButton("Transparent", &buttons[3]);

	glm::vec2 buttonSet = glm::vec2(0, -1); // Bool, iteration num
	for (int i = 3; i > -1; i++)
	{
		if (buttonSet.x == 1)
		{
			buttons[i] = false;
		}
		else
		{
			if (buttonSet.x == 1)
			{
				buttonSet = glm::vec2(1, i);
			}
		}
	}

	switch ((int)buttonSet.y)
	{
	case 0:
	{
		m_renderMode = RendereringMode::RENDER_OPAQUE;
		break;
	}
	case 1:
	{
		m_renderMode = RendereringMode::RENDER_CUTOUT;
		break;
	}
	case 2:
	{
		m_renderMode = RendereringMode::RENDER_FADE;
		break;
	}
	case 3:
	{
		m_renderMode = RendereringMode::RENDER_TRANSPARENT;
		break;
	}
	default:
		break;
	}


	// Textures
	ImGui::ColorEdit4("Albedo", glm::value_ptr(*m_albedo.GetOverrideColour()));
	ImGui::SameLine();
	ImGui::InputText("Image Path", m_albedo.GetFilePath(), sizeof(char) * 256);
	ImGui::ColorEdit4("Metalic", glm::value_ptr(*m_metalic.GetOverrideColour()));
	ImGui::SameLine();
	ImGui::InputText("Image Path", m_metalic.GetFilePath(), sizeof(char) * 256);
	ImGui::ColorEdit4("Normal", glm::value_ptr(*m_normal.GetOverrideColour()));
	ImGui::SameLine();
	ImGui::InputText("Image Path", m_normal.GetFilePath(), sizeof(char) * 256);
	ImGui::ColorEdit4("Height", glm::value_ptr(*m_height.GetOverrideColour()));
	ImGui::SameLine();
	ImGui::InputText("Image Path", m_height.GetFilePath(), sizeof(char) * 256);
}
