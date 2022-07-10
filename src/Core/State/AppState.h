#pragma once
#include "Window/Window.h"
#include "Renderer/Renderer.h"

namespace LearnVulkan {
	class AppState {
	public:
		static Window window;
		static Renderer renderer;
		static void Init();
		static void Destroy();
	private:
	};
}
