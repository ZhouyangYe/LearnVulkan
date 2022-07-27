#pragma once
#include "EventsHandler/EventsHandler.h";
#include "Renderer/Renderer.h"
#include "World/World.h"
#include "Cursor.h"

namespace LearnVulkan {
	class Application {
	public:
		Application();
		~Application();
		void Run();
	private:
		World world;
		EventsHandler eventsHandler;
	};
}
