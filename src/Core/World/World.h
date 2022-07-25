#pragma once
#include "Core/Common.h"
#include "Core/State/GameState.h"

namespace LearnVulkan {
	class World {
	public:
		World();
		~World();

		void Update();
	private:
		uint32_t _frameNumber{ 0 };
	};
}
