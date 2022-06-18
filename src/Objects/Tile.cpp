#include "pch.h"

#include "Objects/Tile.h"
#include "Renderer/Renderer.h"
#include "Core/Application.h"

#include <ImGui/imgui.h>
#include "glm/gtc/matrix_transform.hpp"

Tile::Tile(const std::string& name, int north, int south, int west, int east, uint8_t initialConfigurations)
	: _name(name), _weight(1), _initialConfigurations(initialConfigurations) {
	_model.reset(new Model("assets/models/" + name + ".obj"));

	_imGuiDirectionalLight.Direction = glm::vec3(-1.0f, 5.0f, 2.0f);
	_imGuiDirectionalLight.Color = glm::vec3(0.8f, 0.8f, 0.8f);
	_imGuiDirectionalLight.AmbientColor = glm::vec3(0.1f, 0.1f, 0.1f);
	_imGuiFrameBuffer.reset(new FrameBuffer(Application::GetWindow().GetWidth(), Application::GetWindow().GetHeight()));
	_imGuiTransform = glm::mat4(1.0f);
	_imGuiCamera = Camera();
	_imGuiCamera.SetPosition(glm::vec3(0.0f, 5.0f, 5.0f));
	_imGuiCamera.SetRotation(glm::vec2(-90.0f, 45.0f));
	_conncetions = std::array<int, 4>();
	_conncetions[0] = north;
	_conncetions[1] = east;
	_conncetions[2] = south;
	_conncetions[3] = west;
}

void Tile::Draw(std::shared_ptr<Shader> shader, glm::vec3 position, int configuration) {
	if (_model.get() == nullptr) {
		return;
	}
	glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), (configuration & 4) ? glm::pi<float>() / 2.0f : 0, glm::vec3(0, 1, 0));
	glm::mat4 translation = glm::translate(glm::mat4(1.0f), position);
	glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3((configuration & 1) ? -1.0f : 1.0f, 1.0f, (configuration & 2) ? -1.0f : 1.0f));
	_model->Draw(shader, translation * rotation * scale);
}

void Tile::ImGuiRender(DeltaTime deltaTime) {
	_imGuiTransform = glm::rotate(_imGuiTransform, (float)deltaTime * 0.5f, glm::vec3(0.0f, 1.0f, 0.0f));
	_imGuiFrameBuffer->Bind();
	RenderCommand::SetClearColor({ 0.1f, 0.2f, 0.3f, 1.0f });
	RenderCommand::Clear();
	Renderer::BeginScene(_imGuiCamera, _imGuiDirectionalLight);
	_model->Draw(Application::GetShaderLibrary().Get("Shader"), _imGuiTransform);
	Renderer::EndScene();
	_imGuiFrameBuffer->Unbind();
	ImGui::Image((void*)_imGuiFrameBuffer->GetColorAttachment(), ImVec2(80.0f, 90.0f), ImVec2(0.4f, 0.8f), ImVec2(0.6f, 0.4f));
	ImGui::SameLine();
	ImGui::SliderInt(_name.c_str(), &_weight, 1, 100);
}

bool Tile::ValidateConnection(uint8_t configuration, uint8_t direction, std::shared_ptr<Tile> other, uint8_t otherConfiguration) {
	return GetConnectionType(configuration, direction) == other->GetConnectionType(otherConfiguration, direction + 2);
}

int Tile::GetConnectionType(uint8_t configuration, uint8_t direction) {
	if (configuration & 4) {
		direction += 1;
	}
	if (configuration & 1 && direction & 1) {
		direction += 2;
	}
	if (configuration & 2 && !(direction & 1)) {
		direction += 2;
	}
	return _conncetions[direction % 4];
}
