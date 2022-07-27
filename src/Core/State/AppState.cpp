#include "AppState.h"

namespace LearnVulkan {
	Window AppState::window;
	Renderer AppState::renderer;
	Pipeline AppState::pipeline(&AppState::renderer.device, &AppState::renderer.commandBuffer._renderPass);
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
		camera.Init({ { windowProps.width, windowProps.height }, 200.0f, { 0.f, 0.f, 0.f } });

		// create pipelines
		PosColorNormalVertex::Init();
		pipeline
			// first layout
			.init_layout()                                         // layouts 0
			.init_vertex_layout(PosColorNormalVertex::layout)      // vertex_layouts 0
			.add_pipeline("shader1")                               // pipelines 0
			.add_pipeline("shader2");                              // pipelines 1
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
