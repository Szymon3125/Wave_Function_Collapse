#pragma	once

#include "Core/Layer.h"

#include "Objects/Map.h"

#include "Renderer/Camera.h"
#include "Renderer/DirectionalLight.h"
#include "Renderer/FrameBuffer.h"
#include "Renderer/Model.h"


struct ImVec4;

class MapLayer : public Layer {

public:
	MapLayer();

	virtual void OnUpdate(DeltaTime deltaTime);
	virtual void OnImGuiRender();
	virtual void OnEvent(Event& event);

private:
	ImVec4 ImGuiImageUV(float imageRatio, float windowRatio);

	std::shared_ptr<FrameBuffer> _frameBuffer;
	std::shared_ptr<Shader> _shader;

	std::shared_ptr<Map> _map;

	DirectionalLight _directionalLight;
	Camera _camera;
	glm::vec3 _cameraPosition;

	float _fps;
	float _time;
	int _sizeX;
	int _sizeY;

};