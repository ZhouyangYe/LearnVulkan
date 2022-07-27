#pragma once
#include "Core/Common.h";
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"
#include "Window/Window.h"
#include "Core/State/GameState.h"
#include "Core/World/World.h";

namespace LearnVulkan
{
	class EventsHandler
	{
	public:
		World* world;

		EventsHandler(World* world);
		~EventsHandler();

		void handleKeyPress(KeyPressEvent& event);
		void handleMouseMove(MouseMoveEvent& event);
		void handleMouseWheel(MouseWheelEvent& event);
	};
}
