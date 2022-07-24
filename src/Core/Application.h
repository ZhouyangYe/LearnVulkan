#pragma once
#include "EventsHandler.h";
#include "State/AppState.h"
#include "Renderer/Renderer.h"

namespace LearnVulkan {
	class Application {
	public:
		Application();
		~Application();
		void Run();
	private:
		uint32_t _frameNumber{ 0 };
	};
}
