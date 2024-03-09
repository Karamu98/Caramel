#pragma once
#include <imgui.h>

namespace Caramel
{
	class ImGuiImpl
	{
	public:
		ImGuiImpl() {};

		virtual void Init(class RenderAPI* renderAPI) = 0;
		virtual void Shutdown() = 0;
		virtual void NewFrame() = 0;
		virtual void Render(ImDrawData* drawData) = 0;
	};
}


