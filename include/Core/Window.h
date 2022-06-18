#pragma once

#include "pch.h"

#include "Events/Event.h"

#include <GLFW/glfw3.h>

struct WindowProps {
	std::string Title;
	unsigned int Width;
	unsigned int Height;

	WindowProps(const std::string& title = "New window", unsigned int width = 1200, unsigned int height = 675)
		: Title(title), Width(width), Height(height) {}
};

class Window {

public:
	using EventCallbackFn = std::function<void(Event&)>;

	Window(const WindowProps& props = WindowProps());
	virtual ~Window();

	void OnUpdate();

	inline unsigned int GetWidth() const { return _data.Width; }
	inline unsigned int GetHeight() const { return _data.Height; }

	inline void SetEventCallback(const EventCallbackFn& eventCallback) { _data.EventCallback = eventCallback; }
	void SetVSync(bool anable);
	bool IsVSync() const;

	inline virtual GLFWwindow* GetNativeWindow() const { return _window; }

private:
	virtual void Init(const WindowProps& props);
	virtual void Shutdown();

	GLFWwindow* _window;

	struct WindowData {
		std::string Title;
		unsigned int Width, Height;
		bool VSync;
		std::function<void(Event&)> EventCallback;
	};

	WindowData _data;
};