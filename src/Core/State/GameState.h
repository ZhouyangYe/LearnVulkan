#pragma once
#include "AppState.h"

namespace LearnVulkan {
	class GameState {
	public:
		static VertexBuffer triangleBuffer;

		struct Triangle {
			static uint32_t vertice_num;
			static VertexBuffer* vertex_buffer;
			static uint32_t selectedIndex;
			static uint32_t selectedPipelineIndex;
			static std::vector<uint32_t> pipelines;
			glm::vec3 coord{ 0.0f, 0.0f, 0.0f };

			static void setSelectedIndex(uint32_t index);
		};
		static Triangle triangle;

		static void Init();

		static void Destroy();
	};
}
