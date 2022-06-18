#pragma once

#include <glm/glm.hpp>

class Camera {
	
public:
	Camera(float fov = 45.0f, float ratio = 16.0f / 9.0f, float near = 0.1f, float far = 100.0f);

	const glm::vec3& GetPosition() const { return _position; }
	void SetPosition(const glm::vec3& position) { _position = position; _recalculateViewMatrix(); }
	
	glm::vec2 getRotation() const { return _rotation; }
	void SetRotation(glm::vec2 rotation) { _rotation = rotation; _recalculateViewMatrix(); }

	const glm::mat4& GetProjectionMatrix() const { return _projectionMatrix; }
	const glm::mat4& GetViewMatrix() const { return _viewMatrix; }
	const glm::mat4& GetViewProjectionMatrix() const { return _viewProjectionMatrix; }

private:
	void _recalculateViewMatrix();

	glm::mat4 _projectionMatrix;
	glm::mat4 _viewMatrix;
	glm::mat4 _viewProjectionMatrix;

	glm::vec3 _position = { 0.0f, 0.0f, 0.0f };
	glm::vec2 _rotation = { 0.0f, 0.0f };

};