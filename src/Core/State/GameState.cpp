#include "GameState.h"

namespace LearnVulkan {
	uint32_t GameState::Triangle::vertice_num = 3;
	uint32_t GameState::Triangle::selectedIndex;
	uint32_t GameState::Triangle::selectedPipelineIndex;
	std::vector<uint32_t> GameState::Triangle::pipelines;
	void GameState::Triangle::setSelectedIndex(uint32_t index)
	{
		selectedIndex = index;
		selectedPipelineIndex = pipelines[index];
	}
	GameState::Triangle GameState::triangle;

	VertexBuffer GameState::triangleBuffer;

	void GameState::Init()
	{
		// TODO: move vertice to game state
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

		AppState::renderer.device.upload_mesh(triangleBuffer, vertices.data(), 3 * sizeof(AppState::PosColorNormalVertex));

		Triangle::pipelines.push_back(0);
		Triangle::pipelines.push_back(1);
	}

	void GameState::Destroy()
	{
		AppState::renderer.device.destroy_buffer(triangleBuffer);
	}
}
