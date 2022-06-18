#include "pch.h"

#include "Core/Application.h"

#include "Renderer/Renderer.h"

#include "Layers/MapLayer.h"

Application* Application::_instance = nullptr;

Application::Application() {
	ASSERT(!_instance, "Application already exists!");
	_instance = this;
	_window = std::unique_ptr<Window>(new Window());
	_window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));

	_textureLibrary.reset(new TextureLibrary());
	_shaderLibrary.reset(new ShaderLibrary());

	Renderer::Init();

	_imGuiLayer = new ImGuiLayer();
	PushOverlay(_imGuiLayer);
	PushLayer(new MapLayer());
}

Application::~Application() {}

void Application::PushLayer(Layer* layer) {
	_layerStack.PushLayer(layer);
	layer->OnAttach();
}

void Application::PushOverlay(Layer* overlay) {
	_layerStack.PushOverlay(overlay);
	overlay->OnAttach();
}

void Application::Run() {
	while (_running) {

		float time = (float)glfwGetTime();
		DeltaTime deltaTime = time - _lastFrameTime;
		_lastFrameTime = time;

		for (Layer* layer : _layerStack) {
			layer->OnUpdate(deltaTime);
		}

		_imGuiLayer->Begin();
		for (Layer* layer : _layerStack) {
			layer->OnImGuiRender();
		}
		_imGuiLayer->End();

		_window->OnUpdate();
	}
}

void Application::OnEvent(Event& e) {
	EventDispatcher dispatcher(e);
	dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowClosed));

	for (std::vector<Layer*>::iterator it = _layerStack.end(); it != _layerStack.begin(); ) {
		(*--it)->OnEvent(e);
		if (e.Handled) {
			break;
		}
	}
}

bool Application::OnWindowClosed(WindowCloseEvent& e) {
	_running = false;
	return true;
}