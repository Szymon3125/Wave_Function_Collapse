#pragma once

#include "Core/Layer.h"
#include "Core/DeltaTime.h"

#include "Renderer/Camera.h"
#include "Renderer/FrameBuffer.h"
#include "Renderer/Model.h"
#include "Renderer/Shader.h"

#include "Events/MouseEvent.h"

#include <ImGui/imgui.h>

class TestLayer : public Layer {

public:
	TestLayer();

	virtual void OnUpdate(DeltaTime deltaTime);
	virtual void OnImGuiRender();
	virtual void OnEvent(Event& event);


private:
	bool SwitchRotation(MouseButtonPressedEvent& event);
	ImVec4 CalculateImageUV(float imageRatio, float windowRatio);

	std::shared_ptr<FrameBuffer> _frameBuffer;

	std::shared_ptr<Shader> _shader;
	std::shared_ptr<Model> _model;
	Camera _camera;

	glm::vec3 _cameraPosition;
	float _cameraMoveSpeed = 5.0f;

	glm::vec2 _cameraRotation = { 0.0f, 0.0f };
	float _cameraRotationSpeed = 20000.0f;
	bool _rotating = false;

	glm::vec3 _position;

	char* _player;
	size_t _playerSize;

};