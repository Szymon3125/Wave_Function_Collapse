#include "pch.h"

#include "Core/LayerStack.h"

LayerStack::LayerStack() {}

LayerStack::~LayerStack() {
	for (Layer* layer : _layers) {
		delete layer;
	}
}

void LayerStack::PushLayer(Layer* layer) {
	_layers.emplace(_layers.begin() + _layerInsertIndex, layer);
	_layerInsertIndex++;
}

void LayerStack::PushOverlay(Layer* overlay) {
	_layers.emplace_back(overlay);
}

void LayerStack::PopLayer(Layer* layer) {
	std::vector<Layer*>::iterator it = std::find(_layers.begin(), _layers.end(), layer);
	if (it != _layers.end()) {
		_layers.erase(it);
		_layerInsertIndex--;
	}
}

void LayerStack::PopOverlay(Layer* overlay) {
	std::vector<Layer*>::iterator it = std::find(_layers.begin(), _layers.end(), overlay);
	if (it != _layers.end()) {
		_layers.erase(it);
	}
}
