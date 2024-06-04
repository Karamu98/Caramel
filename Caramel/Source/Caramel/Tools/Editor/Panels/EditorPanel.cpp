#include "clpch.h"
#include "EditorPanel.h"

namespace Caramel
{
	void EditorPanel::Render()
	{
		if (m_isOpen)
		{
			ImGui::Begin(m_name.c_str(), &m_isOpen);
			InternalRender();
			ImGui::End();
		}
	}

	void EditorPanel::RenderListOption()
	{

	}
}