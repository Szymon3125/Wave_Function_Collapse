#include "pch.h"

#include "Renderer/VertexArray.h"

#include <glad/glad.h>

static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type) {
	switch (type) {
		case ShaderDataType::Float:		return GL_FLOAT;
		case ShaderDataType::Float2:	return GL_FLOAT;
		case ShaderDataType::Float3:	return GL_FLOAT;
		case ShaderDataType::Float4:	return GL_FLOAT;
		case ShaderDataType::Mat3:		return GL_FLOAT;
		case ShaderDataType::Mat4:		return GL_FLOAT;
		case ShaderDataType::Int:		return GL_INT;
		case ShaderDataType::Int2:		return GL_INT;
		case ShaderDataType::Int3:		return GL_INT;
		case ShaderDataType::Int4:		return GL_INT;
		case ShaderDataType::Bool:		return GL_BOOL;
	}
	ASSERT(false, "Unknown ShaderDataType!");
	return 0;
}

VertexArray::VertexArray() {
	glGenVertexArrays(1, &_rendererID);
	glBindVertexArray(_rendererID);
}

VertexArray::~VertexArray() {
	glDeleteVertexArrays(1, &_rendererID);
}

void VertexArray::Bind() const {
	glBindVertexArray(_rendererID);
}

void VertexArray::Uubind() const {
	glBindVertexArray(0);
}

void VertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) {
	glBindVertexArray(_rendererID);
	vertexBuffer->Bind();

	ASSERT(vertexBuffer->GetLayout().GetElements().size(), "VertexBuffer has no layouyt");

	int index = 0;
	for (const auto& element : vertexBuffer->GetLayout()) {
		glEnableVertexAttribArray(index);
		glVertexAttribPointer(
			index,
			element.GetComponentCount(),
			ShaderDataTypeToOpenGLBaseType(element.Type),
			element.Normalized ? GL_TRUE : GL_FALSE,
			vertexBuffer->GetLayout().GetStride(),
			(const void*)element.Offset
		);
		index++;
	}

	_vertexBuffers.push_back(vertexBuffer);
}

void VertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) {
	glBindVertexArray(_rendererID);
	indexBuffer->Bind();

	_indexBuffer = indexBuffer;
}
