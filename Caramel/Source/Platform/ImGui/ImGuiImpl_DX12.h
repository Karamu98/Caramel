#pragma once

#include <Core/ImGuiImpl.h>

namespace Caramel
{
	class ImGuiImpl_DX12 : public ImGuiImpl
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


