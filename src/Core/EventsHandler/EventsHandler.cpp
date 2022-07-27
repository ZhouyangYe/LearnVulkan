#include "EventsHandler.h"

namespace LearnVulkan
{
	EventsHandler::EventsHandler(World* world) : world(world) {}

	EventsHandler::~EventsHandler() {}

	void EventsHandler::handleKeyPress(KeyPressEvent& event)
	{
		switch(event.keyCode) {
			case Key::Y:
				if (event.action == KeyPressEvent::ACTION::KEY_DOWN) {
					GameState::Triangle::setSelectedIndex((GameState::Triangle::selectedPipelineIndex + 1) % 2);
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
			case Key::Space:
				if (event.action == KeyPressEvent::ACTION::KEY_DOWN)
					AppState::camera.moveUp = true;
				else if (event.action == KeyPressEvent::ACTION::KEY_UP)
					AppState::camera.moveUp = false;
				break;
			case Key::Z:
				if (event.action == KeyPressEvent::ACTION::KEY_DOWN)
					AppState::camera.moveDown = true;
				else if (event.action == KeyPressEvent::ACTION::KEY_UP)
					AppState::camera.moveDown = false;
				break;
			case Key::A:
				if (event.action == KeyPressEvent::ACTION::KEY_DOWN)
					AppState::camera.moveLeft = true;
				else if (event.action == KeyPressEvent::ACTION::KEY_UP)
					AppState::camera.moveLeft = false;
				break;
			case Key::D:
				if (event.action == KeyPressEvent::ACTION::KEY_DOWN)
					AppState::camera.moveRight = true;
				else if (event.action == KeyPressEvent::ACTION::KEY_UP)
					AppState::camera.moveRight = false;
				break;
			case Key::W:
				if (event.action == KeyPressEvent::ACTION::KEY_DOWN)
					AppState::camera.moveForward = true;
				else if (event.action == KeyPressEvent::ACTION::KEY_UP)
					AppState::camera.moveForward = false;
				break;
			case Key::S:
				if (event.action == KeyPressEvent::ACTION::KEY_DOWN)
					AppState::camera.moveBackward = true;
				else if (event.action == KeyPressEvent::ACTION::KEY_UP)
					AppState::camera.moveBackward = false;
				break;
			default:
				break;
		}
	}

	void EventsHandler::handleMouseMove(MouseMoveEvent& event)
	{
		AppState::cursor.updateCoord({ event.xPos, event.yPos });
		AppState::camera.Rotate(AppState::cursor.getDeltaCoord());
	}

	void EventsHandler::handleMouseWheel(MouseWheelEvent& event)
	{
	}
}
