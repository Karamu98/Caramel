#pragma once

#include "Caramel/Core/Base.h"

namespace Caramel 
{
	// Virtual class to be created in dirivitives
	class Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();

		virtual void OnAttach() {} // This is similar to Init
		virtual void OnDetach() {} // This is similar to Destroy
		virtual void OnUpdate() {} 
		virtual void OnImGuiRender() {}
		virtual void OnEvent(Event& event) {} // Allows for event handling

		inline const std::string& GetName() const { return m_debugName; }
	protected:
		std::string m_debugName; // This is for debug purposes
	};

}