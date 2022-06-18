#pragma once

#include "Renderer/Camera.h"
#include "Renderer/DirectionalLight.h"
#include "Renderer/Shader.h"
#include "Renderer/VertexArray.h"
#include "Renderer/Model.h"
#include "Renderer/RenderCommand.h"

class Renderer {

public:
	static void Init();

	static void BeginScene(Camera& camera, DirectionalLight& directionalLight);
	static void EndScene();

	static void Submit(const std::shared_ptr<VertexArray>& vertexArray, const std::shared_ptr<Shader>& shader, const glm::mat4& transform = glm::mat4(1.0f));

private:
	struct SceneData {
		glm::mat4 ViewProjectionMatrix;
		glm::vec3 LightDirection;
		glm::vec3 LightColor;
		glm::vec3 AmbientColor;
	};
	static SceneData* _sceneData;

};