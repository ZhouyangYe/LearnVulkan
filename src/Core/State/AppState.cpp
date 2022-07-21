#include "AppState.h"

namespace LearnVulkan {
	Window AppState::window;
	Renderer AppState::renderer;

	void AppState::Init() {
		Window::WindowProps props;
		AppState::window.Init(props);

		float clearColor[4] = { 1.0f, 0.3f, 0.6f, 1.0f };
		Renderer::RendererProps renderProps(&AppState::window, clearColor);
		AppState::renderer.Init(renderProps);
	}

	void AppState::Destroy() {
		window.Destroy();
		renderer.Destroy();
	}
}
