#pragma once

#include <Core/ImGui/ImGuiRendererImpl.h>

namespace Caramel
{
	class ImGuiRendererImpl_DX12 : public ImGuiRendererImpl
	{
	public:
		// Inherited via ImGuiImpl
		void Init(class RenderAPI* renderAPI) override;
		void Shutdown() override;
		void NewFrame() override;
		void Render(ImDrawData* drawData) override;

	private:
		class RenderAPI_DX12* m_dxAPI;
	};
}


