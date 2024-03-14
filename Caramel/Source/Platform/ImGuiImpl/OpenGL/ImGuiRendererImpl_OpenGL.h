#pragma once

#include "Core/ImGui/ImGuiRendererImpl.h"

namespace Caramel
{
	class ImGuiRendererImpl_OpenGL : public ImGuiRendererImpl
	{
		// Inherited via ImGuiRendererImpl
		void Init(RenderAPI* renderAPI) override;
		void Shutdown() override;
		void NewFrame() override;
		void Render(ImDrawData* drawData) override;
	};
}


