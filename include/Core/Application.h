#pragma once

#include "Core/DeltaTime.h"
#include "Core/ImGuiLayer.h"
#include "Core/LayerStack.h"
#include "Core/Window.h"

#include "Renderer/Shader.h"
#include "Renderer/Texture.h"

#include "Events/ApplicationEvent.h"

class Application {

public:
	Application();
	virtual ~Application();

	void Run();
	void OnEvent(Event& e);
	void Close() { _running = false; }

	void PushLayer(Layer* layer);
	void PushOverlay(Layer* overlay);

	inline static Application& Get() { return *_instance; }
	inline static Window& GetWindow() { return *(_instance->_window); }
	inline static ShaderLibrary& GetShaderLibrary() { return *(_instance->_shaderLibrary); }
	inline static TextureLibrary& GetTextureLibrary() { return *(_instance->_textureLibrary); }

private:
	bool OnWindowClosed(WindowCloseEvent& e);

	std::unique_ptr<Window> _window;

	std::unique_ptr<ShaderLibrary> _shaderLibrary;
	std::unique_ptr<TextureLibrary> _textureLibrary;

	bool _running = true;
	LayerStack _layerStack;
	ImGuiLayer* _imGuiLayer;

	float _lastFrameTime = 0.0f;

	static Application* _instance;

};