#pragma once

#include "Caramel/Core/Base.h"
#include "Layer.h"

#include <vector>

namespace Caramel 
{
	// LayerStack is owned by Application
	class LayerStack
	{
	public:
		LayerStack();
		~LayerStack();

		void PushLayer(Layer* a_layer); // Adds a layer to the front of the "stack"
		void PushOverlay(Layer* a_overlay); // Adds an overlay to the back of the "stack"
		void PopLayer(Layer* a_layer); // Removes a layer
		void PopOverlay(Layer* a_overlay); // Removes an overlay

		std::vector<Layer*>::iterator begin() { return m_layers.begin(); }
		std::vector<Layer*>::iterator end() { return m_layers.end(); }
	private:
		std::vector<Layer*> m_layers; // List of pointers to layers, popped layers will not be deleted
		unsigned int m_layerInsertIndex = 0; // An index into where to put layers (before overlays)
	};

}