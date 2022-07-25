#include "Time.h"

#define INTERVAL 1.0f

namespace LearnVulkan
{
	Time::Time(Window* window) : window(window) {}

	Time::~Time() {}

	const double& Time::getDeltaTime()
	{
		return deltaTime;
	}

	const double Time::getCurrentTime()
	{
		return window->getCurrentTime();
	}

	void Time::updateTime()
	{
		double currentTime = window->getCurrentTime();
		deltaTime = currentTime - prevTime;
		prevTime = currentTime;
	}

	void Time::updateFps()
	{
		double currentTime = window->getCurrentTime();
		fpsDeltaTime = currentTime - fpsPrevTime;
		if (fpsDeltaTime >= INTERVAL || fps == 0)
		{
			fpsPrevTime = currentTime;
			fps = round(1 / deltaTime);
		}
	}

	const float& Time::getFps()
	{
		return fps;
	}

	void Time::Init()
	{
		prevTime = window->getCurrentTime();
	}
}
