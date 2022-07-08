#include "AppState.h"

namespace LearnVulkan {
	Window* AppState::window = nullptr;

	void AppState::initWindow(Window::WindowProps& props) {
		window = new Window(props);
		window->Init();
	}

	void AppState::Destroy() {
		delete window;
	}
}
