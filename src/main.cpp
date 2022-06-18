#include "pch.h"

#include "Core/Application.h"

int main() {
	Log::Init();
	Application* app = new Application();
	app->Run();
	delete app;
	return 0;
}