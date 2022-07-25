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
						GameState::Triangle::setSelectedIndex((GameState::Triangle::selectedIndex + 1) % 2);
					}
					break;
				case Key::H:
					if (event.action != KeyPressEvent::ACTION::KEY_DOWN)
						break;
					if (AppState::cursor.hideCursor)
						AppState::cursor.show();
					else
						AppState::cursor.hide();
						AppState::cursor.hideCursor = !AppState::cursor.hideCursor;
					break;
				default:
					break;
			}
		}

		void handleMouseMove(MouseMoveEvent& event)
		{
			AppState::cursor.updateCoord({ event.xPos, event.yPos });
			AppState::camera.Rotate(AppState::cursor.getDeltaCoord());
		}

		void handleMouseWheel(MouseWheelEvent& event)
		{
		}
	}
}
