#pragma once
#include "Core/Common.h";
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"
#include "Window/Window.h"
#include "Core/State/GameState.h"

namespace LearnVulkan
{
	namespace EventHandler
	{
		void handleKeyPress(KeyPressEvent& event);
		void handleMouseMove(MouseMoveEvent& event);
		void handleMouseWheel(MouseWheelEvent& event);
	}
}