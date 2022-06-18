#pragma once

#include "Renderer/Buffer.h"
#include "Renderer/Shader.h"
#include "Renderer/Texture.h"
#include "Renderer/VertexArray.h"

#include <vector>
#include <glm/glm.hpp>

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

class Mesh {
	
public:
	Mesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices, std::vector<std::shared_ptr<Texture>> textures);
	void Draw(const std::shared_ptr<Shader> shader, const glm::mat4& transform);

private:
	void SetupMesh();

	std::vector<Vertex> _vertices;
	std::vector<uint32_t> _indices;
	std::vector<std::shared_ptr<Texture>> _textures;

	std::shared_ptr<VertexBuffer> _vertexBuffer;
	std::shared_ptr<IndexBuffer> _indexBuffer;
	std::shared_ptr<VertexArray> _vertexArray;

};