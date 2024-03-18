#pragma once

#include "Core/Events/Event.h"
#include "Core/Timestep.h"

namespace Caramel
{
	class Layer
	{
	public:
		friend class LayerStack;
		Layer(const std::string& debugName = "Layer");
		virtual ~Layer() noexcept;

		virtual void OnAttach() {};
		virtual void OnDetach() {};
		virtual void OnUpdate(Timestep ts) {};
		virtual void OnImGuiRender() {};
		virtual void OnEvent(Event& event) {};

		inline const std::string& GetName() const { return m_debugName; }

	protected:
		std::string m_debugName;
	};

}
