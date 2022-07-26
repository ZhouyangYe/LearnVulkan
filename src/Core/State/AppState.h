#pragma once
#include "Window/Window.h"
#include "Core/Cursor.h"
#include "Core/Time.h"
#include "Renderer/Renderer.h"
#include "Camera/Camera.h"

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
					.add_binding()
					.end();
			};
		};

		static Window window;
		static Renderer renderer;
		static Pipeline pipeline;
		static Cursor cursor;
		static Time time;
		static Camera camera;
		static void Init();
		static void Wait();
		static void Destroy();
	private:
	};
}
