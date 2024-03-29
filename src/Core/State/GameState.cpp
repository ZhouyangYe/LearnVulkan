#include "GameState.h"

namespace LearnVulkan {
	// triangle
	Buffer GameState::Triangle::buffer;
	VkPipelineLayout GameState::Triangle::pipelineLayout;
	VkPipeline GameState::Triangle::pipeline;
	std::vector<AppState::PosColorNormalUVVertex> GameState::Triangle::vertices;
	std::vector<uint32_t> GameState::Triangle::pipelines;
	uint32_t GameState::Triangle::selectedPipelineIndex = 0;
	void GameState::Triangle::setSelectedIndex(uint32_t index)
	{
		selectedPipelineIndex = index;
		pipeline = AppState::pipeline.pipelines[Triangle::pipelines[index]];
	}

	// monkey
	Buffer GameState::Monkey::buffer;
	VkPipelineLayout GameState::Monkey::pipelineLayout;
	VkPipeline GameState::Monkey::pipeline;
	std::vector<AppState::PosColorNormalUVVertex> GameState::Monkey::vertices;

	// terrain
	Buffer GameState::Terrain::buffer;
	VkPipelineLayout GameState::Terrain::pipelineLayout;
	VkPipeline GameState::Terrain::pipeline;
	std::vector<AppState::PosColorNormalUVVertex> GameState::Terrain::vertices;

	void GameState::Init()
	{
		// set up triangle data
		Triangle::vertices.resize(3);
		// vertex positions
		Triangle::vertices[0].position = {  1.f, 1.f, 0.0f };
		Triangle::vertices[1].position = { -1.f, 1.f, 0.0f };
		Triangle::vertices[2].position = {  0.f,-1.f, 0.0f };

		// vertex colors, cyan
		Triangle::vertices[0].color = { 0.f, 1.f, 1.0f };
		Triangle::vertices[1].color = { 0.f, 1.f, 1.0f };
		Triangle::vertices[2].color = { 0.f, 1.f, 1.0f };

		GPUData triangle_data{ Triangle::vertices.data(), 3 * sizeof(AppState::PosColorNormalUVVertex)  };
		AppState::renderer.upload_vertex_data(Triangle::buffer, triangle_data);

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

			GPUData monkey_data{ Monkey::vertices.data(), Monkey::vertices.size() * sizeof(AppState::PosColorNormalUVVertex) };
			AppState::renderer.upload_vertex_data(Monkey::buffer, monkey_data);
		}
		catch (Error err) {
			Logger::console->error(err.desc);
		}

		// set up terrain data
		try {
			Terrain::pipeline = AppState::pipeline.pipelines[0];
			Terrain::pipelineLayout = AppState::pipeline.layouts[0];
			std::vector<ModelTools::Mesh> mesh = ModelTools::loadModel(fmt::format("{0}\\{1}", RESOURCE_FOLDER, "models\\lost_empire.obj"));
			for (auto iter = mesh.begin(); iter != mesh.end(); ++iter) {
				Terrain::vertices.emplace_back(iter->position, iter->normal, iter->normal, iter->uv);
			}

			GPUData terrain_data{ Terrain::vertices.data(), Terrain::vertices.size() * sizeof(AppState::PosColorNormalUVVertex) };
			AppState::renderer.upload_vertex_data(Terrain::buffer, terrain_data);
		}
		catch (Error err) {
			Logger::console->error(err.desc);
		}
	}

	void GameState::Destroy()
	{
		AppState::renderer.device.destroy_buffer(Triangle::buffer);
		AppState::renderer.device.destroy_buffer(Monkey::buffer);
		AppState::renderer.device.destroy_buffer(Terrain::buffer);
	}
}
