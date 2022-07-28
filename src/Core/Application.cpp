#include "Application.h"

namespace LearnVulkan {
	Application::Application() : eventsHandler(&world) {
		AppState::Init();

		AppState::window.setEventCallback([&](Event& event)
			{
				switch (event.type)
				{
				case Event::TYPE::KEY_PRESS:
					eventsHandler.handleKeyPress((KeyPressEvent&)event);
					break;
				case Event::TYPE::MOUSE_MOVE:
					eventsHandler.handleMouseMove((MouseMoveEvent&)event);
					break;
				case Event::TYPE::MOUSE_WHEEL:
					eventsHandler.handleMouseWheel((MouseWheelEvent&)event);
					break;
				default:
					break;
				}
			});

		GameState::Init();

		world.Init();

		AppState::time.Init();
	}

	Application::~Application() {
		AppState::Wait();
		world.Destroy();
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
			AppState::renderer.Draw();

			if (AppState::cursor.hideCursor) AppState::cursor.reset();

			AppState::camera.End();
			AppState::window.End();
		}
	}
}
