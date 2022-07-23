#include "App.h"

namespace LearnVulkan {
	App::App() {
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
	}

	App::~App() {
		AppState::Destroy();
	}

	void App::Run() {
		while (!AppState::window.shouldWindowBeClosed()) {
			if (AppState::window.isMinimized()) {
				return;
			}

			AppState::window.Begin();

			AppState::renderer.Draw(AppState::pipeline, AppState::triangleBuffer);

			AppState::window.End();
		}
	}
}
