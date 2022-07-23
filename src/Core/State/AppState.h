#pragma once
#include "Window/Window.h"
#include "Renderer/Renderer.h"

namespace LearnVulkan {
	class AppState {
	public:
		struct PosColorNormalVertex
		{
			glm::vec3 position;
			glm::vec3 normal;
			glm::vec3 color;

			static VertexLayout layout;
			static void Init()
			{
				layout
					.begin()
					.add(VK_FORMAT_R32G32B32_SFLOAT, sizeof(float) * 3)
					.add(VK_FORMAT_R32G32B32_SFLOAT, sizeof(float) * 3)
					.add(VK_FORMAT_R32G32B32_SFLOAT, sizeof(float) * 3)
					.end();;
			};
		};
		static VertexBuffer triangleBuffer;

		static Window window;
		static Renderer renderer;
		static Pipeline pipeline;
		static void Init();
		static void Destroy();
	private:
	};
}
