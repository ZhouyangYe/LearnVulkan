#include "AppState.h"

namespace LearnVulkan {
	Window AppState::window;
	Renderer AppState::renderer;
	Pipeline AppState::pipeline(&AppState::renderer.device, &AppState::renderer.commandBuffer._renderPass, &AppState::renderer.descriptor);
	Cursor AppState::cursor(&AppState::window);
	Time AppState::time(&AppState::window);
	Camera AppState::camera(&AppState::time);

	std::vector<Texture> AppState::textures;

	VertexLayout AppState::PosColorNormalUVVertex::layout;

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

		// load textures
		// TODO: find a way to manage layouts
		textures.emplace_back(AppState::renderer, fmt::format("{0}\\{1}", RESOURCE_FOLDER, "textures\\lost_empire-RGBA.png").c_str());  // -- texture 1
		for (auto iter = textures.begin(); iter != textures.end(); ++iter) {
			iter->create_set();
		}

		// create pipelines
		PosColorNormalUVVertex::Init();
		pipeline
			.init_layout(renderer.descriptor._textureLayouts)      // -- layouts 0
			.init_vertex_layout(PosColorNormalUVVertex::layout)
			.add_pipeline("shader1")                               // -- pipelines 0
			.add_pipeline("shader2");                              // -- pipelines 1
	}

	void AppState::Wait()
	{
		renderer.Wait();
	}

	void AppState::Destroy() {
		window.Destroy();
		for (auto iter = textures.begin(); iter != textures.end(); ++iter) {
			iter->Destroy();
		}
		pipeline.Destroy();
		renderer.Destroy();
	}
}
