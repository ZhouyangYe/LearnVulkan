#pragma once
#include "Core/Common.h"
#include "Core/State/GameState.h"

namespace LearnVulkan {
	class World {
	public:
		World();
		~World();

		void Init();
		void Update();
		void Destroy();
	private:
		uint32_t _frameNumber{ 0 };
		std::vector<Renderer::Renderable> objects;
	};
}
