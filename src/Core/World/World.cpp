#include "World.h"

namespace LearnVulkan {
	World::World() {}

	World::~World() {}

	void World::Init()
	{

	}

	void World::Update()
	{
		/*glm::mat4 model1 = glm::rotate(glm::mat4{ 1.0f }, glm::radians(_frameNumber * 0.4f), { 0.f, 1.0f, 0.f });
		model1 *= glm::translate(glm::mat4{ 1.0f }, { 0.f, 6.0f, 0.f});
		glm::mat4 model2 = glm::translate(glm::mat4{ 1.0f }, { 0.f, 6.0f, 6.0f });
		model2 *= glm::rotate(glm::mat4{ 1.0f }, glm::radians(_frameNumber * -0.4f), { 0.f, 1.0f, 0.f });

		float framed = (_frameNumber / 120.f);
		AppState::renderer.sceneData.ambientColor = { sin(framed),0,cos(framed),1 };

		AppState::renderer.submit(GameState::Monkey::buffer, GameState::Monkey::pipelineLayout, GameState::Monkey::pipeline, GameState::Monkey::vertices.size(), model1);
		AppState::renderer.submit(GameState::Monkey::buffer, GameState::Monkey::pipelineLayout, GameState::Monkey::pipeline, GameState::Monkey::vertices.size(), model2);*/

		glm::mat4 model(1);
		AppState::renderer.submit(GameState::Terrain::buffer, GameState::Terrain::pipelineLayout, GameState::Terrain::pipeline, GameState::Terrain::vertices.size(), model, AppState::textures[0].textureSet);

		_frameNumber++;
	}

	void World::Destroy()
	{

	}
}
