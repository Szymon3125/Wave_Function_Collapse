#include "pch.h"

#include "Core/Application.h"
#include "Core/Input.h"

#include "Layers/MapLayer.h"

#include "Renderer/Renderer.h"
#include "Renderer/RenderCommand.h"

#include <ImGui/imgui.h>

MapLayer::MapLayer() : _camera() {
    _directionalLight.Direction = glm::vec3(1.0f, -5.0f, 2.0f);
    _directionalLight.Color = glm::vec3(0.8f, 0.8f, 0.8f);
    _directionalLight.AmbientColor = glm::vec3(0.1f, 0.1f, 0.1f);

    _cameraPosition = glm::vec3(0.0f, 5.0f, 5.0f);
    _camera.SetPosition(_cameraPosition);
    _camera.SetRotation(glm::vec2(-90.0f, 45.0f));

    _frameBuffer.reset(new FrameBuffer(Application::GetWindow().GetWidth(), Application::GetWindow().GetHeight()));
    _shader.reset(new Shader("assets/shaders/Shader.glsl"));
    Application::GetShaderLibrary().Add(_shader);

    _map.reset(new Map());
    _sizeX = 15;
    _sizeY = 15;
}

void MapLayer::OnUpdate(DeltaTime deltaTime) {

    _fps = 1.0f / (float)deltaTime;
    
    // Input
    if (Input::IsKeyPressed(GLFW_KEY_A)) { _cameraPosition.x -= 5 * deltaTime.GetSeconds(); }
    else if (Input::IsKeyPressed(GLFW_KEY_D)) { _cameraPosition.x += 5 * deltaTime.GetSeconds(); }
    if (Input::IsKeyPressed(GLFW_KEY_W)) { _cameraPosition.z -= 5 * deltaTime.GetSeconds(); }
    else if (Input::IsKeyPressed(GLFW_KEY_S)) { _cameraPosition.z += 5 * deltaTime.GetSeconds(); }
    _camera.SetPosition(_cameraPosition);

    // Render
    _frameBuffer->Bind();
    RenderCommand::SetClearColor({ 0.1f, 0.2f, 0.3f, 1.0f });
    RenderCommand::Clear();

    Renderer::BeginScene(_camera, _directionalLight);

    _shader->Bind();
    _map->Draw(_shader);

    Renderer::EndScene();
    _frameBuffer->Unbind();
}

void MapLayer::OnImGuiRender() {
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

    //if (ImGui::BeginMenuBar()) {
    //    if (ImGui::BeginMenu("File")) {
    //        if (ImGui::MenuItem("Close")) {
    //            Application::Get().Close();
    //        }
    //        ImGui::EndMenu();
    //    }
    //    ImGui::EndMenuBar();
    //}
    ImGui::End();

    ImGui::Begin("Map");
    ImGui::BeginChild("Map");
    ImVec2 wsize = ImGui::GetWindowSize();
    ImVec4 uv = ImGuiImageUV(16.0f / 9.0f, wsize.x / wsize.y);
    ImGui::Image((void*)_frameBuffer->GetColorAttachment(), wsize, ImVec2(uv.x, uv.y), ImVec2(uv.z, uv.w));
    ImGui::EndChild();
    ImGui::End();

    ImGui::Begin("Menu");
    ImGui::Text("FPS: %.1f", _fps);
    ImGui::Separator();
    ImGui::SliderInt("X", &_sizeX, 5, 50);
    ImGui::SliderInt("Y", &_sizeY, 5, 50);
    if (ImGui::Button("Generate")) {
        _map->Generate(_sizeX, _sizeY);
    }
    ImGui::Separator();
    _map->ImGuiRender(1.0f / _fps);
    ImGui::End();
}

void MapLayer::OnEvent(Event& event) {

}

ImVec4 MapLayer::ImGuiImageUV(float imageRatio, float windowRatio) {
    if (windowRatio > imageRatio) {
        float offset = (1.0f - imageRatio / windowRatio) / 2.0f;
        return ImVec4(0.0f, 1.0f - offset, 1.0f, offset);
    }
    float offset = (1.0f - windowRatio / imageRatio) / 2.0f;
    return ImVec4(offset, 1.0f, 1.0f - offset, 0.0f);
}