#include "AppState.h"

namespace LearnVulkan {
	Window AppState::window;
	Renderer AppState::renderer;
	Pipeline AppState::pipeline;
	Cursor AppState::cursor(&AppState::window);
	Time AppState::time(&AppState::window);
	Camera AppState::camera(&AppState::time);

	VertexLayout AppState::PosColorNormalVertex::layout;

	void AppState::Init() {
		// initialize window
		Window::WindowProps windowProps;
		window.Init(windowProps);

		// initialize renderer
		float clearColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
		Renderer::RendererProps renderProps(&window, clearColor);
		renderer.Init(renderProps);

		// init camera
		camera.Init({ { windowProps.width, windowProps.height }, 200.0f, { 0.f,0.f,-2.f } });

		// create pipelines
		PosColorNormalVertex::Init();
		renderer.initPipeline(pipeline, PosColorNormalVertex::layout);
		pipeline.add_pipeline("C:\\BrighT\\Dev\\LearnVulkan\\resources\\shaders\\bin\\vert1.spv", "C:\\BrighT\\Dev\\LearnVulkan\\resources\\shaders\\bin\\frag1.spv");
		pipeline.add_pipeline("C:\\BrighT\\Dev\\LearnVulkan\\resources\\shaders\\bin\\vert2.spv", "C:\\BrighT\\Dev\\LearnVulkan\\resources\\shaders\\bin\\frag2.spv");
	}

	void AppState::Wait()
	{
		renderer.Wait();
	}

	void AppState::Destroy() {
		window.Destroy();
		pipeline.Destroy();
		renderer.Destroy();
	}
}
