#pragma once
#include "VK_HEADER.h"
#include "Device.h"
#include "CommandBuffer.h"
#include "SwapChain.h"
#include "Synchronization.h"
#include "Pipeline.h"
#include "VertexLayout.h"
#include "MeshBuffer.h"

namespace LearnVulkan {
	class Renderer {
	public:
		struct RendererProps
		{
			Window* window;
			float clearColor[4]{ 0.0f, 0.0f, 0.0f, 0.0f };

			RendererProps(Window* window, float color[4])
				: window(window), clearColor{ color[0], color[1], color[2], color[3] }
			{}
		};

		bool _isInitialized{ false };

		Device device;
		CommandBuffer commandBuffer;
		SwapChain swapChain;
		Synchronization sync;

		Renderer();
		~Renderer();

		// initializes everything in the engine
		void Init(RendererProps& props);
		// wait for gpu to finish its work
		void Wait();
		// shuts down the engine
		void Destroy();
		// draw loop
		void Draw(Pipeline& pipeline, VertexBuffer& vBuffer, uint32_t& selectedPipelineIndex, glm::mat4& model, uint32_t& vertice_num);

		static void setViewTransform(glm::mat4& view, glm::mat4& projection);
	private:
		static glm::mat4 projection_view;
	};
}
