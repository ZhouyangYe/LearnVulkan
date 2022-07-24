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
	}

	Application::~Application() {
		AppState::Destroy();
	}

	void Application::Run() {
		while (!AppState::window.shouldWindowBeClosed()) {
			if (AppState::window.isMinimized()) {
				continue;
			}

			AppState::window.Begin();


			// TODO: use real camera
			glm::vec3 camPos = { 0.f,0.f,-2.f };

			glm::mat4 view = glm::translate(glm::mat4(1.f), camPos);
			//camera projection
			glm::mat4 projection = glm::perspective(glm::radians(70.f), 1700.f / 900.f, 0.1f, 200.0f);
			projection[1][1] *= -1;
			//model rotation
			glm::mat4 model = glm::rotate(glm::mat4{ 1.0f }, glm::radians(_frameNumber * 0.4f), glm::vec3(0, 1, 0));
			_frameNumber++;

			//calculate final mesh matrix
			glm::mat4 mesh_matrix = projection * view * model;
			AppState::pushConstantData.render_matrix = mesh_matrix;


			AppState::renderer.Draw(AppState::triangleBuffer);

			AppState::window.End();
		}
	}
}
