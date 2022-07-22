#include "AppState.h"

namespace LearnVulkan {
	Window AppState::window;
	Renderer AppState::renderer;
	Pipeline AppState::pipeline;

	void AppState::Init() {
		// initialize window
		Window::WindowProps props;
		AppState::window.Init(props);

		// initialize renderer
		float clearColor[4] = { 1.0f, 0.3f, 0.6f, 1.0f };
		Renderer::RendererProps renderProps(&AppState::window, clearColor);
		AppState::renderer.Init(renderProps);

		renderer.initPipeline(pipeline);
		pipeline.add_pipeline("C:\\BrighT\\Dev\\LearnVulkan\\resources\\shaders\\bin\\vert1.spv", "C:\\BrighT\\Dev\\LearnVulkan\\resources\\shaders\\bin\\frag1.spv");
		pipeline.add_pipeline("C:\\BrighT\\Dev\\LearnVulkan\\resources\\shaders\\bin\\vert2.spv", "C:\\BrighT\\Dev\\LearnVulkan\\resources\\shaders\\bin\\frag2.spv");
	}

	void AppState::Destroy() {
		window.Destroy();
		renderer.Wait();
		pipeline.Destroy();
		renderer.Destroy();
	}
}
