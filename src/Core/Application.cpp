#include "Application.h"

namespace LearnVulkan {
	Application::Application() {
		AppState::Init();

		AppState::window.setEventCallback([](Event& event)
			{
				switch (event.type)
				{
				case Event::TYPE::KEY_PRESS:
					EventHandler::handleKeyPress((KeyPressEvent&)event);
					break;
				case Event::TYPE::MOUSE_MOVE:
					EventHandler::handleMouseMove((MouseMoveEvent&)event);
					break;
				case Event::TYPE::MOUSE_WHEEL:
					EventHandler::handleMouseWheel((MouseWheelEvent&)event);
					break;
				default:
					break;
				}
			});

		GameState::Init();

		AppState::time.Init();
	}

	Application::~Application() {
		AppState::Wait();
		GameState::Destroy();
		AppState::Destroy();
	}

	void Application::Run() {
		while (!AppState::window.shouldWindowBeClosed()) {
			if (AppState::window.isMinimized()) {
				continue;
			}

			AppState::time.updateTime();
			// if (Info::shouldShowInfo()) Time::updateFps();

			AppState::window.Begin();
			AppState::camera.Begin();

			world.Update();

			if (AppState::cursor.hideCursor) AppState::cursor.reset();

			AppState::camera.End();
			AppState::window.End();
		}
	}
}
