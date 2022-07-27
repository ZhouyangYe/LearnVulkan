#include "GameState.h"

namespace LearnVulkan {
	// triangle
	VertexBuffer GameState::Triangle::buffer;
	std::vector<uint32_t> GameState::Triangle::pipelines;
	uint32_t GameState::Triangle::vertice_num = 3;
	VkPipelineLayout GameState::Triangle::pipelineLayout;
	VkPipeline GameState::Triangle::pipeline;
	uint32_t GameState::Triangle::selectedPipelineIndex = 0;
	void GameState::Triangle::setSelectedIndex(uint32_t index)
	{
		selectedPipelineIndex = index;
		pipeline = AppState::pipeline.pipelines[Triangle::pipelines[index]];
	}

	GameState::Triangle GameState::triangle;

	// monkey
	VertexBuffer GameState::Monkey::buffer;
	VkPipelineLayout GameState::Monkey::pipelineLayout;
	VkPipeline GameState::Monkey::pipeline;
	std::vector<AppState::PosColorNormalVertex> GameState::Monkey::vertices;

	GameState::Monkey GameState::monkey;
	GameState::Monkey GameState::monkey2({0.f, -6.0f, 0.f});


	void GameState::Init()
	{
		// set up triangle data
		std::vector<AppState::PosColorNormalVertex> vertices;
		vertices.resize(3);
		// vertex positions
		vertices[0].position = { 1.f, 1.f, 0.0f };
		vertices[1].position = { -1.f, 1.f, 0.0f };
		vertices[2].position = { 0.f,-1.f, 0.0f };

		// vertex colors, cyan
		vertices[0].color = { 0.f, 1.f, 1.0f };
		vertices[1].color = { 0.f, 1.f, 1.0f };
		vertices[2].color = { 0.f, 1.f, 1.0f };

		AppState::renderer.device.upload_mesh(Triangle::buffer, vertices.data(), 3 * sizeof(AppState::PosColorNormalVertex));

		Triangle::pipelines.push_back(0);
		Triangle::pipelines.push_back(1);
		Triangle::pipeline = AppState::pipeline.pipelines[0];
		Triangle::pipelineLayout = AppState::pipeline.layouts[0];


		// set up monkey data
		try {
			Monkey::pipeline = AppState::pipeline.pipelines[0];
			Monkey::pipelineLayout = AppState::pipeline.layouts[0];
			std::vector<ModelTools::Mesh> mesh = ModelTools::loadModel(fmt::format("{0}\\{1}", RESOURCE_FOLDER, "models\\monkey_smooth.obj"));
			for (auto iter = mesh.begin(); iter != mesh.end(); ++iter) {
				Monkey::vertices.emplace_back(iter->position, iter->normal, iter->normal);
			}

			AppState::renderer.device.upload_mesh(Monkey::buffer, Monkey::vertices.data(), Monkey::vertices.size() * sizeof(AppState::PosColorNormalVertex));
		}
		catch (Error err) {
			Logger::console->error(err.desc);
		}
	}

	void GameState::Destroy()
	{
		AppState::renderer.device.destroy_buffer(Triangle::buffer);
		AppState::renderer.device.destroy_buffer(Monkey::buffer);
	}
}
