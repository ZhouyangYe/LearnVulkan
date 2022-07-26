#pragma once
#include "AppState.h"
#include "Model/Tools.h"

namespace LearnVulkan {
	class GameState {
	public:
		struct Triangle {
			static VertexBuffer buffer;
			static uint32_t vertice_num;
			static uint32_t selectedIndex;
			static uint32_t selectedPipelineIndex;
			static std::vector<uint32_t> pipelines;
			glm::vec3 coord{ 0.0f, 1.0f, 0.0f };

			static void setSelectedIndex(uint32_t index);
		};
		static Triangle triangle;

		struct Monkey {
			static VertexBuffer buffer;
			static uint32_t selectedPipelineIndex;
			glm::vec3 coord{ 0.0f, 6.0f, 0.0f };
			std::vector<AppState::PosColorNormalVertex> vertices;
		};
		static Monkey monkey;

		static void Init();

		static void Destroy();
	};
}
