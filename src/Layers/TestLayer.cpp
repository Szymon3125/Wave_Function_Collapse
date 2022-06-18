#include "pch.h"

#include "Layers/TestLayer.h"

#include "Core/Application.h"
#include "Core/Input.h"

#include "Renderer/Renderer.h"
#include "Renderer/RenderCommand.h"

#include <glm/gtc/matrix_transform.hpp>

TestLayer::TestLayer() : Layer("TestLayer"), _camera(), _cameraPosition(0.0f), _position(0.0f) {
    _model.reset(new Model("assets/models/pawn.obj"));
    _shader.reset(new Shader("assets/shaders/Shader.glsl"));
    _frameBuffer.reset(new FrameBuffer(Application::GetWindow().GetWidth(), Application::GetWindow().GetHeight()));
    _playerSize = 128;
    _player = new char[_playerSize];
    _player[0] = '\0';
}

void TestLayer::OnUpdate(DeltaTime deltaTime) {
    if (Input::IsKeyPressed(GLFW_KEY_A)) { _cameraPosition.x -= _cameraMoveSpeed * deltaTime.GetSeconds(); }
    else if (Input::IsKeyPressed(GLFW_KEY_D)) { _cameraPosition.x += _cameraMoveSpeed * deltaTime.GetSeconds(); }
    if (Input::IsKeyPressed(GLFW_KEY_LEFT_SHIFT)) { _cameraPosition.y -= _cameraMoveSpeed * deltaTime.GetSeconds(); }
    else if (Input::IsKeyPressed(GLFW_KEY_SPACE)) { _cameraPosition.y += _cameraMoveSpeed * deltaTime.GetSeconds(); }
    if (Input::IsKeyPressed(GLFW_KEY_W)) { _cameraPosition.z -= _cameraMoveSpeed * deltaTime.GetSeconds(); }
    else if (Input::IsKeyPressed(GLFW_KEY_S)) { _cameraPosition.z += _cameraMoveSpeed * deltaTime.GetSeconds(); }

    if (Input::IsKeyPressed(GLFW_KEY_J)) { _position.x -= _cameraMoveSpeed * deltaTime.GetSeconds(); }
    else if (Input::IsKeyPressed(GLFW_KEY_L)) { _position.x += _cameraMoveSpeed * deltaTime.GetSeconds(); }
    if (Input::IsKeyPressed(GLFW_KEY_K)) { _position.y -= _cameraMoveSpeed * deltaTime.GetSeconds(); }
    else if (Input::IsKeyPressed(GLFW_KEY_I)) { _position.y += _cameraMoveSpeed * deltaTime.GetSeconds(); }

    if (_rotating) {
        uint32_t width = Application::GetWindow().GetWidth();
        uint32_t height = Application::GetWindow().GetHeight();
        _cameraRotation.x += _cameraRotationSpeed * (float)(Input::GetMouseX() - width / 2) / width * deltaTime.GetSeconds();
        _cameraRotation.y += _cameraRotationSpeed * (float)(Input::GetMouseY() - height / 2) / height * deltaTime.GetSeconds();
        _cameraRotation.x = _cameraRotation.x > -180.0f ? (_cameraRotation.x < 180.0f ? _cameraRotation.x : _cameraRotation.x - 360.0f) : _cameraRotation.x + 360.0f;
        _cameraRotation.y = _cameraRotation.y > -89.999f ? (_cameraRotation.y < 89.999f ? _cameraRotation.y : 89.999f) : -89.999f;
        glfwSetCursorPos(Application::GetWindow().GetNativeWindow(), width / 2, height / 2);
    }

    _frameBuffer->Bind();
    RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
    RenderCommand::Clear();

    _camera.SetRotation(_cameraRotation);
    _camera.SetPosition(_cameraPosition);

    // Renderer::BeginScene(_camera); + oœwietlenie

    glm::mat4 transform = glm::translate(glm::mat4(1.0f), _position);
    _model->Draw(_shader, transform);

    Renderer::EndScene();
    _frameBuffer->Unbind();
}

void TestLayer::OnImGuiRender() {
    static bool open = false;
    static bool opt_fullscreen = true;
    static bool opt_padding = false;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    if (opt_fullscreen) {
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    }
    else {
        dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
    }

    // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
    // and handle the pass-thru hole, so we ask Begin() to not render a background.
    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
        window_flags |= ImGuiWindowFlags_NoBackground;

    // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
    // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
    // all active windows docked into it will lose their parent and become undocked.
    // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
    // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
    if (!opt_padding)
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("DockSpace", &open, window_flags);
    if (!opt_padding)
        ImGui::PopStyleVar();

    if (opt_fullscreen)
        ImGui::PopStyleVar(2);

    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }

    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Close")) {
                Application::Get().Close();
            }
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }
    ImGui::End();

    // Szachownica
    ImGui::Begin("Game");
    ImGui::BeginChild("GameRender");
    ImVec2 wsize = ImGui::GetWindowSize();
    ImVec4 uv = CalculateImageUV(16.0f / 9.0f, wsize.x / wsize.y);
    ImGui::Image((void*)_frameBuffer->GetColorAttachment(), wsize, ImVec2(uv.x, uv.y), ImVec2(uv.z, uv.w));
    ImGui::EndChild();
    ImGui::End();

    // Gracze
    ImGui::Begin("Players");
    ImGuiInputTextCallback;
    ImGui::InputText("Player", _player, _playerSize);
    if (ImGui::Button("Select")) {
        LOG_TRACE("{0}", _player);
    }
    ImGui::End();
}

void TestLayer::OnEvent(Event& event) {
    EventDispatcher dispatcher(event);
    dispatcher.Dispatch<MouseButtonPressedEvent>(BIND_EVENT_FN(TestLayer::SwitchRotation));
}

bool TestLayer::SwitchRotation(MouseButtonPressedEvent& event) {
    if (event.GetMouseButton() == 1) {
        if (_rotating) {
            _rotating = false;
            glfwSetCursorPos(Application::GetWindow().GetNativeWindow(), Application::GetWindow().GetWidth() / 2, Application::GetWindow().GetHeight() / 2);
            glfwSetInputMode(Application::GetWindow().GetNativeWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
        else {
            _rotating = true;
            glfwSetCursorPos(Application::GetWindow().GetNativeWindow(), Application::GetWindow().GetWidth() / 2, Application::GetWindow().GetHeight() / 2);
            glfwSetInputMode(Application::GetWindow().GetNativeWindow(), GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
        }
        return true;
    }
    return false;
}

ImVec4 TestLayer::CalculateImageUV(float imageRatio, float windowRatio) {
    if (windowRatio > imageRatio) {
        float offset = (1.0f - imageRatio / windowRatio) / 2.0f;
        return ImVec4(0.0f, 1.0f - offset, 1.0f, offset);
    }
    float offset = (1.0f - windowRatio / imageRatio) / 2.0f;
    return ImVec4(offset, 1.0f, 1.0f - offset, 0.0f);
}
