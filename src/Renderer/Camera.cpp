#include "pch.h"

#include "Renderer/Camera.h"

#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(float fov, float ratio, float n, float f) {
	_projectionMatrix = glm::perspective(glm::radians(fov), ratio, n, f);
	_recalculateViewMatrix();
}

void Camera::_recalculateViewMatrix() {
	glm::vec3 direction(
		glm::cos(glm::radians(_rotation.x)) * glm::cos(glm::radians(_rotation.y)),
		-glm::sin(glm::radians(_rotation.y)),
		glm::sin(glm::radians(_rotation.x)) * glm::cos(glm::radians(_rotation.y))
	);
	glm::vec3 right = glm::normalize(glm::cross({ 0.0f, 1.0f, 0.0f }, direction));
	glm::vec3 up = glm::cross(direction, right);
	_viewMatrix = glm::lookAt(_position, _position + direction, up);
	_viewProjectionMatrix = _projectionMatrix * _viewMatrix;
}
