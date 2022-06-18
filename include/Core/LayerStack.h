#pragma once

#include "Layer.h"

#include <vector>

class LayerStack {

public:
	LayerStack();
	virtual ~LayerStack();

	void PushLayer(Layer* layer);
	void PushOverlay(Layer* overlay);
	void PopLayer(Layer* layer);
	void PopOverlay(Layer* overlay);

	std::vector<Layer*>::iterator begin() { return _layers.begin(); }
	std::vector<Layer*>::iterator end() { return _layers.end(); }

private:
	std::vector<Layer*> _layers;
	unsigned int _layerInsertIndex = 0;

};