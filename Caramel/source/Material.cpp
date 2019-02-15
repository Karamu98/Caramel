#include "Material.h"
#include "glm/gtc/type_ptr.hpp"
#include "imgui.h"

Material::Material()
{
	m_albedo = new Texture();
	m_metalic = new Texture();
	m_normal = new Texture();
	m_height = new Texture();
}

Material::~Material()
{

}

void Material::OnGUI()
{
	//bool buttons[4];
	//ImGui::RadioButton("Opaque", &buttons[0]);
	//ImGui::RadioButton("Cutout", &buttons[1]);
	//ImGui::RadioButton("Fade", &buttons[2]);
	//ImGui::RadioButton("Transparent", &buttons[3]);

	//glm::vec2 buttonSet = glm::vec2(0, -1); // Bool, iteration num
	//for (int i = 3; i > -1; i++)
	//{
	//	if (buttonSet.x == 1)
	//	{
	//		buttons[i] = false;
	//	}
	//	else
	//	{
	//		if (buttonSet.x == 1)
	//		{
	//			buttonSet = glm::vec2(1, i);
	//		}
	//	}
	//}

	//switch ((int)buttonSet.y)
	//{
	//case 0:
	//{
	//	m_renderMode = RendereringMode::RENDER_OPAQUE;
	//	break;
	//}
	//case 1:
	//{
	//	m_renderMode = RendereringMode::RENDER_CUTOUT;
	//	break;
	//}
	//case 2:
	//{
	//	m_renderMode = RendereringMode::RENDER_FADE;
	//	break;
	//}
	//case 3:
	//{
	//	m_renderMode = RendereringMode::RENDER_TRANSPARENT;
	//	break;
	//}
	//default:
	//	break;
	//}

	// Textures
	m_albedo->OnGUI("Albedo");
	m_metalic->OnGUI("Metalic");
	m_normal->OnGUI("Normal");
	m_height->OnGUI("Height");
}
