#pragma once

#include <Core/Layers/Layer.h>

namespace Caramel
{
	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		virtual void OnImGuiRender() override;

		void Begin();
		void End();

	protected:
		class ImGuiImpl* m_imGuiImpl;
	};
}


