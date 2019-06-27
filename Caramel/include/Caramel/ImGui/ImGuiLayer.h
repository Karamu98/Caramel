#pragma once

#include "clpch.h"
#include "Caramel/Core/Layer.h"

namespace Caramel 
{
	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		ImGuiLayer(const std::string& name);
		virtual ~ImGuiLayer();

		void StartFrame();
		void EndFrame();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;
	private:
		float m_Time = 0.0f;
	};

}
