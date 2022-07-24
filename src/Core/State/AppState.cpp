#include "AppState.h"

namespace LearnVulkan {
	Window AppState::window;
	Renderer AppState::renderer;
	Pipeline AppState::pipeline;
	VertexBuffer AppState::triangleBuffer;
	AppState::MeshPushConstants AppState::pushConstantData;

	VertexLayout AppState::PosColorNormalVertex::layout;

	void AppState::Init() {
		// initialize window
		Window::WindowProps props;
		window.Init(props);

		// initialize renderer
		float clearColor[4] = { 1.0f, 0.3f, 0.6f, 1.0f };
		Renderer::RendererProps renderProps(&window, clearColor);
		renderer.Init(renderProps);

		PosColorNormalVertex::Init();

		// TODO: move vertice to game state
		std::vector<PosColorNormalVertex> vertices;
		vertices.resize(3);
		// vertex positions
		vertices[0].position = { 1.f, 1.f, 0.0f };
		vertices[1].position = { -1.f, 1.f, 0.0f };
		vertices[2].position = { 0.f,-1.f, 0.0f };

		// vertex colors, cyan
		vertices[0].color = { 0.f, 1.f, 1.0f };
		vertices[1].color = { 0.f, 1.f, 1.0f };
		vertices[2].color = { 0.f, 1.f, 1.0f };

		triangleBuffer.upload_mesh(renderer.device, vertices.data(), 3 * sizeof(PosColorNormalVertex), 3);

		renderer.initPipeline(pipeline, PosColorNormalVertex::layout);
		pipeline.add_pipeline("C:\\BrighT\\Dev\\LearnVulkan\\resources\\shaders\\bin\\vert1.spv", "C:\\BrighT\\Dev\\LearnVulkan\\resources\\shaders\\bin\\frag1.spv");
		pipeline.add_pipeline("C:\\BrighT\\Dev\\LearnVulkan\\resources\\shaders\\bin\\vert2.spv", "C:\\BrighT\\Dev\\LearnVulkan\\resources\\shaders\\bin\\frag2.spv");

		triangleBuffer.bind_pipeline(&pipeline);
	}

	void AppState::Destroy() {
		window.Destroy();
		renderer.Wait();
		triangleBuffer.Destroy();
		pipeline.Destroy();
		renderer.Destroy();
	}
}
