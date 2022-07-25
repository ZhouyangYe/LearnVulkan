#include "Cursor.h"

namespace LearnVulkan
{
	Cursor::Cursor(Window* window) : window(window) {}

	Cursor::~Cursor() {}

	glm::vec2 Cursor::getDeltaCoord()
	{
		return delta * sensitivity;
	}

	void Cursor::updateCoord(glm::vec2 coord)
	{
		glm::vec2 center = window->getCenter();

		delta = coord - prevCoord;
		prevCoord = hideCursor ? center : coord;
	}

	void Cursor::hide()
	{
		glm::vec2 center = window->getCenter();
		prevCoord = glm::vec2{ center.x, center.y };
		window->hideCursor();
	}

	void Cursor::reset()
	{
		glm::vec2 center = window->getCenter();
		window->setCursorPos(center.x, center.y);
	}

	void Cursor::show()
	{
		window->showCursor();
	}
}
