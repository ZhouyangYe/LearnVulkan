#include "AppState.h"

namespace LearnVulkan {
	Window AppState::window;
	Renderer AppState::renderer;

	void AppState::Init() {
		Window::WindowProps props;
		AppState::window.Init(props);

		AppState::renderer.Init(AppState::window);
	}

	void AppState::Destroy() {
		window.Destroy();
		renderer.Destroy();
	}
}
