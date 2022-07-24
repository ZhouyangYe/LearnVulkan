#include "EventsHandler.h"

namespace LearnVulkan
{
	namespace EventHandler
	{
		void handleKeyPress(KeyPressEvent& event)
		{
			switch(event.keyCode) {
				case Key::Space:
					if (event.action == KeyPressEvent::ACTION::KEY_DOWN) {
						AppState::triangleBuffer.setSelectedPipeline((AppState::triangleBuffer.selectedIndex + 1) % 2);
					}
					break;
				default:
					break;
			}
		}

		void handleMouseMove(MouseMoveEvent& event)
		{
		}

		void handleMouseWheel(MouseWheelEvent& event)
		{
		}
	}
}
