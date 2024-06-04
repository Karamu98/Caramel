#pragma once

#include "Caramel/Core/Layer.h"

namespace Caramel
{
	class ImGuiRendererLayer : public Layer
	{
	public:
		ImGuiRendererLayer();
		~ImGuiRendererLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		void Begin();
		void End();

	protected:
		class ImGuiRendererImpl* m_imGuiRendererImpl;
		class ImGuiWindowImpl* m_imGuiWindowImpl;
	};
}


