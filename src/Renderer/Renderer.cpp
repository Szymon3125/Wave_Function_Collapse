#include "pch.h"

#include "Renderer/Renderer.h"
#include "Renderer/RenderCommand.h"

Renderer::SceneData* Renderer::_sceneData = new Renderer::SceneData();

void Renderer::Init() {
	RenderCommand::Init();
}

void Renderer::BeginScene(Camera& camera, DirectionalLight& directionalLight) {
	_sceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
	_sceneData->LightDirection = directionalLight.Direction;
	_sceneData->LightColor = directionalLight.Color;
	_sceneData->AmbientColor = directionalLight.AmbientColor;
}

void Renderer::EndScene() {}

void Renderer::Submit(const std::shared_ptr<VertexArray>& vertexArray, const std::shared_ptr<Shader>& shader, const glm::mat4& transform) {
	shader->Bind();
	shader->UploadUniformMat4("u_ViewProjection", _sceneData->ViewProjectionMatrix);
	shader->UploadUniformFloat3("u_LightDirection", _sceneData->LightDirection);
	shader->UploadUniformFloat3("u_LightColor", _sceneData->LightColor);
	shader->UploadUniformFloat3("u_AmbientColor", _sceneData->AmbientColor);
	shader->UploadUniformMat4("u_Transform", transform);

	vertexArray->Bind();
	RenderCommand::DrawIndexed(vertexArray);
}