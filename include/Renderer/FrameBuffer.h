#pragma once

class FrameBuffer {

public:
	FrameBuffer(int width, int height);
	virtual ~FrameBuffer();

	void Bind();
	void Unbind();

	uint32_t GetColorAttachment() { return _colorAttachment; }

private:
	uint32_t _rendererID;
	uint32_t _colorAttachment;
	uint32_t _depthAttachment;

	int _width;
	int _height;

};