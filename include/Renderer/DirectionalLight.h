#pragma once

#include <glm/glm.hpp>

struct DirectionalLight {

public:
	glm::vec3 Direction;
	glm::vec3 Color;
	glm::vec3 AmbientColor;

	DirectionalLight(glm::vec3 direction = glm::vec3(0.0f), glm::vec3 color = glm::vec3(0.0f), glm::vec3 ambientColor = glm::vec3(0.0f)) {
		Direction = direction;
		Color = color;
		AmbientColor = ambientColor;
	}

};