#pragma once
#include "VK_HEADER.h"
#include "Device.h"
#include "CommandBuffer.h"
#include "SwapChain.h"
#include "Synchronization.h"
#include "Pipeline.h"
#include "VertexLayout.h"
#include "Buffer.h"
#include "Descriptor.h"

namespace LearnVulkan {
	class Renderer {
	public:
		struct Renderable {
			Buffer vertex_buffer;
			VkPipelineLayout pipelineLayout;
			VkPipeline pipeline;
			uint32_t vertice_num;
			glm::mat4 model;
		};

		struct CameraData {
			glm::mat4 view;
			glm::mat4 projection;
			glm::mat4 projection_view;
		};

		struct SceneData {
			glm::vec4 fogColor; // w is for exponent
			glm::vec4 fogDistances; //x for min, y for max, zw unused.
			glm::vec4 ambientColor;
			glm::vec4 sunlightDirection; //w for sun power
			glm::vec4 sunlightColor;
		};

		struct RendererProps
		{
			Window* window;
			float clearColor[4]{ 0.0f, 0.0f, 0.0f, 0.0f };

			RendererProps(Window* window, float color[4])
				: window(window), clearColor{ color[0], color[1], color[2], color[3] }
			{}
		};

		static SceneData sceneData;

		bool _isInitialized{ false };

		Device device;
		CommandBuffer commandBuffer;
		SwapChain swapChain;
		Synchronization sync;

		Descriptor descriptor;

		Renderer();
		~Renderer();

		// initializes everything in the engine
		void Init(RendererProps& props);
		// wait for gpu to finish its work
		void Wait();
		// shuts down the engine
		void Destroy();
		// draw loop
		void Draw();

		void upload_pushConstants(VkCommandBuffer cmd, VkPipelineLayout pipelineLayout, const void* data);
		void upload_vertex_data(Buffer& buffer, GPUData& data);
		void submit(Buffer& buffer, VkPipelineLayout pipelineLayout, VkPipeline pipeline, uint32_t vertice_num, glm::mat4& model);
		static void setViewTransform(glm::mat4& view, glm::mat4& projection);
	private:
		static CameraData camData;

		uint32_t frameIndex{ 0 };
		std::vector<Renderable> renderable_objects;

		void draw_renderables(VkCommandBuffer cmd, Renderable* first, int count);
	};
}
