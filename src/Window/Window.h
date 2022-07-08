#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "Core/Common.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"

namespace LearnVulkan {
	class Window {
	public:
		struct WindowProps
		{
			std::string title;
			uint32_t width;
			uint32_t height;

			WindowProps(
				const std::string& title = "Learn Vulkan",
				uint32_t width = 800,
				uint32_t height = 600
			)
				: title(title), width(width), height(height)
			{
			}
		};

		struct WindowSize {
			uint32_t width;
			uint32_t height;
			WindowSize(uint32_t width, uint32_t height) : width(width), height(height) {}
		};

		struct WindowData
		{
			std::function<void(Event& e)> eventHandler = nullptr;
			bool framebufferResized = false;
			WindowData() {};
		};

		const uint32_t& Width();
		const uint32_t& Height();
		const std::string& Title();
		const bool& FrameBufferResized();
		const bool& ShouldWindowClose();
		const WindowSize FrameBufferSize();

		void setEventCallback(const std::function<void(Event& e)>& fn);

		void Init();
		void Begin();
		void End();

		Window(WindowProps props) : title(props.title), width(props.width), height(props.height);
		~Window();
	private:
		uint32_t width;
		uint32_t height;
		std::string title;
		GLFWwindow* m_window;
		WindowData m_window_data;
		bool shouldWindowClose = false;
	};
}
