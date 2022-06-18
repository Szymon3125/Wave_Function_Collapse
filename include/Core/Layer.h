#pragma once

#include "Events/Event.h"
#include "Core/DeltaTime.h"

class Layer {

public:
	Layer(const std::string& debugName = "Layer");
	virtual ~Layer();

	virtual void OnAttach() {}
	virtual void OnDetach() {}
	virtual void OnUpdate(DeltaTime deltaTime) {}
	virtual void OnImGuiRender() {}
	virtual void OnEvent(Event& event) {}

	inline const std::string& GetName() const { return _debugName; }

private:
	std::string _debugName;

};