#include "World.h"

namespace LearnVulkan {
	World::World() {}

	World::~World() {}

	void World::Update()
	{
		//model rotation
		glm::mat4 model = glm::rotate(glm::mat4{ 1.0f }, glm::radians(_frameNumber * 0.4f), { 0.f, 1.0f, 0.f });
		model = glm::translate(model, GameState::monkey.coord);
		_frameNumber++;

		uint32_t vertice_num = GameState::monkey.vertices.size();
		AppState::renderer.Draw(GameState::monkey.buffer, GameState::monkey.pipeline, GameState::monkey.pipelineLayout, model, vertice_num);
	}
}
