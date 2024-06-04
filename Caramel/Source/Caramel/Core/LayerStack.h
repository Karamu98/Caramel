#pragma once
#include "Layer.h"

namespace Caramel
{
	class LayerStack
	{
	public:
		LayerStack();
		~LayerStack();

		void TrackLayer(Layer* layer);
		void TrackOverlay(Layer* overlay);
		void UntrackLayer(Layer* layer);
		void UntrackOverlay(Layer* overlay);

		std::vector<Layer*>::iterator begin() { return m_layers.begin(); }
		std::vector<Layer*>::iterator end() { return m_layers.end(); }

	private:
		std::vector<Layer*> m_layers;
		unsigned int m_layerInsertIndex;
	};
}

