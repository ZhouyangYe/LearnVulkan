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
				uint32_t width = 1024,
				uint32_t height = 768
			)
				: title(title), width(width), height(height)
			{
			}
		};

		struct WindowSize
		{
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

		bool isMinimized();
		const std::string& getTitle();
		const bool& isFrameBufferResized();
		const bool& shouldWindowBeClosed();
		const WindowSize getWindowSize();
		const WindowSize getFrameBufferSize();
		void getSurface(VkInstance& instance, VkSurfaceKHR& surface);
		const glm::vec2& getCenter();
		void hideCursor();
		void showCursor();
		void setCursorPos(double width, double height);
		void setWindowSize(WindowSize s);
		double getCurrentTime();

		void setEventCallback(const std::function<void(Event& e)>& fn);

		void Init(WindowProps& props);
		void Destroy();
		void Begin();
		void End();

		Window();
		~Window();
	private:
		GLFWwindow* m_window;
		WindowData m_window_data;

		std::string title;
		bool shouldWindowClose;
		glm::vec2 center;
		WindowSize size;
	};
}
