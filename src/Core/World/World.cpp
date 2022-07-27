#include "World.h"

namespace LearnVulkan {
	World::World() {}

	World::~World() {}

	void World::Init()
	{
		Renderer::Renderable monkey;
		monkey.buffer = GameState::Monkey::buffer;
		monkey.model = glm::mat4{1.0f};
		monkey.pipeline = GameState::Monkey::pipeline;
		monkey.pipelineLayout = GameState::Monkey::pipelineLayout;
		monkey.vertice_num = GameState::Monkey::vertices.size();
		objects.push_back(monkey);
		objects.push_back(monkey);
	}

	void World::Update()
	{
		Renderer::Renderable& monkey1 = objects[0];
		Renderer::Renderable& monkey2 = objects[1];
		monkey1.model = glm::rotate(glm::mat4{ 1.0f }, glm::radians(_frameNumber * 0.4f), { 0.f, 1.0f, 0.f });
		monkey1.model *= glm::translate(glm::mat4{ 1.0f }, { 0.f, 6.0f, 0.f});
		monkey2.model = glm::translate(glm::mat4{ 1.0f }, { 0.f, 6.0f, 6.0f });
		monkey2.model *= glm::rotate(glm::mat4{ 1.0f }, glm::radians(_frameNumber * -0.4f), { 0.f, 1.0f, 0.f });
		_frameNumber++;

		AppState::renderer.Draw(objects);
	}

	void World::Destroy()
	{

	}
}
