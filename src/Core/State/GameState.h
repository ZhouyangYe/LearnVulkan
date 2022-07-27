#pragma once
#include "AppState.h"
#include "Model/Tools.h"

namespace LearnVulkan {
	class GameState {
	public:
		struct Renderable {
			VertexBuffer buffer;
			VkPipelineLayout pipelineLayout;
			VkPipeline pipeline;
			uint32_t vertice_num;
			glm::vec3 coord{ 0.0f, 0.0f, 0.0f };
		};

		struct Triangle {
			static VertexBuffer buffer;
			static VkPipelineLayout pipelineLayout;
			static VkPipeline pipeline;
			static std::vector<AppState::PosColorNormalVertex> vertices;

			static uint32_t selectedPipelineIndex;
			static std::vector<uint32_t> pipelines;

			static void setSelectedIndex(uint32_t index);
		};

		struct Monkey {
			static VertexBuffer buffer;
			static VkPipelineLayout pipelineLayout;
			static VkPipeline pipeline;
			static std::vector<AppState::PosColorNormalVertex> vertices;
		};

		static void Init();

		static void Destroy();
	};
}
