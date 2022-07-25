#pragma once
#include "Window/Window.h"

namespace LearnVulkan
{
	class Cursor
	{
	public:
		bool hideCursor = false;

		Cursor(Window* window);
		~Cursor();

		glm::vec2 getDeltaCoord();
		void updateCoord(glm::vec2 coord);
		void hide();
		void reset();
		void show();
	private:
		Window* window;
		float sensitivity = 0.2f;
		glm::vec2 prevCoord;
		glm::vec2 delta{ 0.0f, 0.0f };
	};
}
