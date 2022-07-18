#pragma once
#include "VK_HEADER.h"
#include "Device.h"
#include "CommandBuffer.h"
#include "SwapChain.h"
#include "Synchronization.h"

namespace LearnVulkan {
	class Renderer {
	public:
		bool _isInitialized{ false };

		Device device;
		CommandBuffer commandBuffer;
		SwapChain swapChain;
		Synchronization sync;

		Renderer();
		~Renderer();

		// initializes everything in the engine
		void Init(Window& window);
		// shuts down the engine
		void Destroy();
		// draw loop
		void Draw();
	private:
	};
}
