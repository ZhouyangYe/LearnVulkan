#pragma once
#include "Window/Window.h"

namespace LearnVulkan {
	class AppState {
	public:
		static Window* window;
		static void initWindow(Window::WindowProps& props);
		static void Destroy();
	private:
	};
}
