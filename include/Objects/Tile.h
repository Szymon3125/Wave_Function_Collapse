#pragma once

#include "Core/DeltaTime.h"
#include "Renderer/Camera.h"
#include "Renderer/DirectionalLight.h"
#include "Renderer/Model.h"
#include "Renderer/FrameBuffer.h"
#include "glm/glm.hpp"

class Tile {

public:
	Tile(const std::string& name, int north, int south, int west, int east, uint8_t initialConfigurations);
	void Draw(std::shared_ptr<Shader> shader, glm::vec3 position, int configuration);
	void ImGuiRender(DeltaTime deltaTime);
	bool ValidateConnection(uint8_t configuration, uint8_t direction, std::shared_ptr<Tile> other, uint8_t otherConfiguration);
	int GetConnectionType(uint8_t configuration, uint8_t direction);
	uint8_t GetInitialConfigurations() { return _initialConfigurations; }
	std::string GetName() { return _name; }
	void setWeight(int weight) { _weight = weight; }
	int GetWeight() { return _weight; }

private:
	uint8_t _initialConfigurations;
	std::string _name;
	std::shared_ptr<Model> _model;
	std::array<int, 4> _conncetions;
	int _weight;

	// manu kafelków
	DirectionalLight _imGuiDirectionalLight;
	std::shared_ptr<FrameBuffer> _imGuiFrameBuffer;
	glm::mat4 _imGuiTransform;
	Camera _imGuiCamera;

};
