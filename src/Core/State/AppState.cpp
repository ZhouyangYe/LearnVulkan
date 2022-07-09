#include "AppState.h"

namespace LearnVulkan {
	Window AppState::window;

	void AppState::Destroy() {
		window.Destroy();
	}
}
