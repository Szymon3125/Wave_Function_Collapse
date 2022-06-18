#pragma once

enum class ShaderDataType {
	None = 0,
	Float, Float2, Float3, Float4,
	Mat3, Mat4,
	Int, Int2, Int3, Int4,
	Bool
};

static uint32_t ShaderDataTypeSize(ShaderDataType type) {
	switch (type) {
		case ShaderDataType::Float:		return 4;
		case ShaderDataType::Float2:	return 4 * 2;
		case ShaderDataType::Float3:	return 4 * 3;
		case ShaderDataType::Float4:	return 4 * 4;
		case ShaderDataType::Mat3:		return 4 * 3 * 3;
		case ShaderDataType::Mat4:		return 4 * 4 * 4;
		case ShaderDataType::Int:		return 4;
		case ShaderDataType::Int2:		return 4 * 2;
		case ShaderDataType::Int3:		return 4 * 3;
		case ShaderDataType::Int4:		return 4 * 4;
		case ShaderDataType::Bool:		return 1;
	}
	ASSERT(false, "Unknown ShaderDataType!");
	return 0;
}

struct BufferElement {

	std::string Name;
	ShaderDataType Type;
	uint32_t Offset;
	uint32_t Size;
	bool Normalized;

	BufferElement(ShaderDataType type, const std::string& name, bool normalized = false) : Name(name), Type(type), Size(ShaderDataTypeSize(type)), Offset(0), Normalized(normalized) {}

	uint32_t GetComponentCount() const {
		switch (Type) {
			case ShaderDataType::Float:		return 1;
			case ShaderDataType::Float2:	return 2;
			case ShaderDataType::Float3:	return 3;
			case ShaderDataType::Float4:	return 4;
			case ShaderDataType::Mat3:		return 3 * 3;
			case ShaderDataType::Mat4:		return 4 * 4;
			case ShaderDataType::Int:		return 1;
			case ShaderDataType::Int2:		return 2;
			case ShaderDataType::Int3:		return 3;
			case ShaderDataType::Int4:		return 4;
			case ShaderDataType::Bool:		return 1;
		}

		ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

};

class BufferLayout {

public:
	BufferLayout() {}
	BufferLayout(const std::initializer_list<BufferElement>& elements) : _elements(elements) {
		CalculateOffsetAndStride();
	}

	inline const std::vector< BufferElement>& GetElements() const { return _elements; }
	inline uint32_t GetStride() const { return _stride; }

	std::vector<BufferElement>::iterator begin() { return _elements.begin(); }
	std::vector<BufferElement>::iterator end() { return _elements.end(); }
	std::vector<BufferElement>::const_iterator begin() const { return _elements.begin(); }
	std::vector<BufferElement>::const_iterator end() const { return _elements.end(); }

protected:
	std::vector<BufferElement> _elements;
	uint32_t _stride = 0;
	void CalculateOffsetAndStride() {
		uint32_t offset = 0;
		_stride = 0;
		for (auto& element : _elements) {
			element.Offset = offset;
			offset += element.Size;
			_stride += element.Size;
		}
	}

};

class VertexBuffer {

public:
	VertexBuffer(float* vertices, uint32_t size);
	virtual ~VertexBuffer();

	void Bind() const;
	void Unbind() const;

	const BufferLayout& GetLayout() const { return _layout; }
	void SetLayout(BufferLayout& layout) { _layout = layout; }

private:
	uint32_t _rendererID;
	BufferLayout _layout;

};

class IndexBuffer {

public:
	IndexBuffer(uint32_t* indices, uint32_t count);
	virtual ~IndexBuffer();

	void Bind() const;
	void Unbind() const;
	uint32_t GetCount() const { return _count; }

private:
	uint32_t _rendererID;
	uint32_t _count;

};