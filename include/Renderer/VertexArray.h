#pragma once

#include "Renderer/Buffer.h"

#include <memory>
#include <vector>

class VertexArray {

public:
	VertexArray();
	virtual ~VertexArray();

	void Bind() const;
	void Uubind() const;

	void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer);
	void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer);

	const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const { return _vertexBuffers; }
	const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const { return _indexBuffer; }

private:
	uint32_t _rendererID;

	std::vector<std::shared_ptr<VertexBuffer>> _vertexBuffers;
	std::shared_ptr<IndexBuffer> _indexBuffer;

};