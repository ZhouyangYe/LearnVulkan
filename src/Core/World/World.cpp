#include "World.h"

namespace LearnVulkan {
	World::World() {}

	World::~World() {}

	void World::Init()
	{

	}

	void World::Update()
	{
		//model rotation
		glm::mat4 model = glm::rotate(glm::mat4{ 1.0f }, glm::radians(_frameNumber * 0.4f), { 0.f, 1.0f, 0.f });
		model = glm::translate(model, { 0.f, 6.0f, 0.f});
		_frameNumber++;

		uint32_t vertice_num = monkey.vertices.size();
		AppState::renderer.Draw(monkey.buffer, monkey.pipeline, monkey.pipelineLayout, model, vertice_num);
	}

	void World::Destroy()
	{

	}
}
