#pragma once
#include "Common.h"
#include "Window/Window.h"

namespace LearnVulkan
{
	class Time
	{
	public:
		Time(Window* window);
		~Time();

		const double& getDeltaTime();
		void updateTime();
		void updateFps();
		const float& getFps();
		const double getCurrentTime();
		void Init();
	private:
		Window* window;
		double deltaTime = 0.0f;
		double prevTime = 0.0f;
		double fpsDeltaTime = 0.0f;
		double fpsPrevTime = 0.0f;
		float fps = 0.0f;
	};
}
