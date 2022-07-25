#include "World.h"

namespace LearnVulkan {
	World::World() {}

	World::~World() {}

	void World::Update()
	{
		//model rotation
		glm::mat4 model = glm::rotate(glm::mat4{ 1.0f }, glm::radians(_frameNumber * 0.4f), glm::vec3(0, 1, 0));
		_frameNumber++;

		AppState::renderer.Draw(AppState::pipeline, *GameState::Triangle::vertex_buffer, GameState::Triangle::selectedPipelineIndex, model, GameState::Triangle::vertice_num);
	}
}
