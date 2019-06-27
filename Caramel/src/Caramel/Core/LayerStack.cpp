#include "clpch.h"
#include "Caramel/Core/LayerStack.h"

namespace Caramel 
{
	LayerStack::LayerStack()
	{
	}

	LayerStack::~LayerStack()
	{
		// All tracked layers will be deleted
		for (Layer* layer : m_layers)
		{
			delete layer;
		}
	}

	void LayerStack::PushLayer(Layer* a_layer)
	{
		// Always put layers at the front 
		m_layers.emplace(m_layers.begin() + m_layerInsertIndex, a_layer);
		m_layerInsertIndex++;
	}

	void LayerStack::PushOverlay(Layer* a_overlay)
	{
		// Always put overlays at the back of the list
		m_layers.emplace_back(a_overlay);
	}

	void LayerStack::PopLayer(Layer* a_layer)
	{
		// Remove layer and adjust inserting index
		auto it = std::find(m_layers.begin(), m_layers.end(), a_layer);
		if (it != m_layers.end())
		{
			m_layers.erase(it);
			m_layerInsertIndex--;
		}

	}

	void LayerStack::PopOverlay(Layer* a_overlay)
	{
		// Remove overlay
		auto it = std::find(m_layers.begin(), m_layers.end(), a_overlay);
		if (it != m_layers.end()) 
		{
			m_layers.erase(it);
		}
	}

}