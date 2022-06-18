#include "pch.h"

#include "Renderer/Mesh.h"
#include "Renderer/Renderer.h"

#include "glm/gtc/matrix_transform.hpp"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices, std::vector<std::shared_ptr<Texture>> textures) {
	_vertices = vertices;
	_indices = indices;
	_textures = textures;
	SetupMesh();
}

void Mesh::Draw(const std::shared_ptr<Shader> shader, const glm::mat4& transform) {
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	for (unsigned int i = 0; i < _textures.size(); i++) {
		std::string number;
		std::string name;
		if (_textures[i]->GetType() == TextureType::Diffuse) {
			name = "Diffuse";
			number = std::to_string(diffuseNr++);
		}
		else if (_textures[i]->GetType() == TextureType::Specular) {
			name = "Specular";
			number = std::to_string(specularNr++);
		}
		_textures[i]->Bind(i);
		shader->UploadUniformInt(("u_Texture" + name + "" + number).c_str(), i);
	}
	glActiveTexture(GL_TEXTURE0);
	Renderer::Submit(_vertexArray, shader, transform);
}

void Mesh::SetupMesh() {
	// number of floats per one vertex;
	int vertexSize = 8;
	_vertexArray.reset(new VertexArray());

	float* vertices = new float[_vertices.size() * vertexSize];
	for (int i = 0; i < _vertices.size(); i++) {
		vertices[i * vertexSize + 0] = _vertices[i].Position.x;
		vertices[i * vertexSize + 1] = _vertices[i].Position.y;
		vertices[i * vertexSize + 2] = _vertices[i].Position.z;
		vertices[i * vertexSize + 3] = _vertices[i].Normal.x;
		vertices[i * vertexSize + 4] = _vertices[i].Normal.y;
		vertices[i * vertexSize + 5] = _vertices[i].Normal.z;
		vertices[i * vertexSize + 6] = _vertices[i].TexCoords.x;
		vertices[i * vertexSize + 7] = _vertices[i].TexCoords.y;
	}
	_vertexBuffer.reset(new VertexBuffer(vertices, (uint32_t)_vertices.size() * vertexSize * sizeof(float)));
	BufferLayout layout = {
		{ ShaderDataType::Float3, "a_Position" },
		{ ShaderDataType::Float3, "a_Normal" },
		{ ShaderDataType::Float2, "a_TexCoord" }
	};
	_vertexBuffer->SetLayout(layout);
	_vertexArray->AddVertexBuffer(_vertexBuffer);

	_indexBuffer.reset(new IndexBuffer(_indices.data(), (uint32_t)_indices.size()));
	_vertexArray->SetIndexBuffer(_indexBuffer);
}


