#pragma once

#include "Core/Events/Event.h"

namespace Caramel
{
	class Layer
	{
	public:
		friend class LayerStack;
		Layer(const std::string& debugName = "Layer");
		virtual ~Layer();

		virtual void OnAttach() {};
		virtual void OnDetach() {};
		virtual void OnUpdate() {};
		virtual void OnImGuiRender() {};
		virtual void OnEvent(Event& event) {};

		inline const std::string& GetName() const { return m_debugName; }

	protected:
		std::string m_debugName;
	};

}
