#include "pch.h"

#include "Core/Input.h"

#include "Core/Application.h"

#include <GLFW/glfw3.h>

bool Input::IsKeyPressed(int keycode) {
	auto window = Application::GetWindow().GetNativeWindow();
	auto state = glfwGetKey(window, keycode);
	return state == GLFW_PRESS || state == GLFW_REPEAT;
}

bool Input::IsMouseButtonPressed(int button) {
	auto window = Application::GetWindow().GetNativeWindow();
	auto state = glfwGetMouseButton(window, button);
	return state == GLFW_PRESS;
}

std::pair<float, float> Input::GetMousePosition() {
	auto window = Application::GetWindow().GetNativeWindow();
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);
	return { (float)xpos, (float)ypos };
}

float Input::GetMouseX() {
	auto [x, y] = Input::GetMousePosition();
	return x;
}

float Input::GetMouseY() {
	auto [x, y] = Input::GetMousePosition();
	return y;
}
