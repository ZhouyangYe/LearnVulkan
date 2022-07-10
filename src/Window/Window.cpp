#include "Window.h"

namespace LearnVulkan {
	const std::string& Window::getTitle() {
		return title;
	}

    const bool& Window::isFrameBufferResized() {
        return m_window_data.framebufferResized;
    }

    const bool& Window::shouldWindowBeClosed() {
        return shouldWindowClose || glfwWindowShouldClose(m_window);
    }

    bool Window::isMinimized() {
        return glfwGetWindowAttrib(m_window, GLFW_ICONIFIED) != 0;
    }

    void Window::getSurface(VkInstance& instance, VkSurfaceKHR& surface) {
        glfwCreateWindowSurface(instance, m_window, nullptr, &surface);
    }

    const Window::WindowSize Window::getWindowSize() {
        int width, height;
        glfwGetWindowSize(m_window, &width, &height);
        return { static_cast<uint32_t>(width), static_cast<uint32_t>(height) };
    }

    const Window::WindowSize Window::getFrameBufferSize() {
        int width, height;
        glfwGetFramebufferSize(m_window, &width, &height);
        return { static_cast<uint32_t>(width), static_cast<uint32_t>(height) };
    }

    void Window::setEventCallback(const std::function<void(Event& e)>& fn) {
        if (!fn) return;
        m_window_data.eventHandler = fn;
    }

	Window::Window() : title("Untitled"), m_window(nullptr), shouldWindowClose(false) {}

    Window::~Window() {}

    void Window::Init(WindowProps& props) {
        title = props.title;

        glfwInit();

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

        m_window = glfwCreateWindow(props.width, props.height, title.c_str(), NULL, NULL);
        if (m_window == NULL) {
            return;
        }

        glfwSetWindowUserPointer(m_window, &m_window_data);

        // Bind keyboard event callback
        glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
            {
                WindowData& windowData = *(WindowData*)glfwGetWindowUserPointer(window);
                if (!windowData.eventHandler) return;

                switch (action)
                {
                case GLFW_PRESS:
                {
                    KeyPressEvent event(Event::TYPE::KEY_PRESS, key, KeyPressEvent::ACTION::KEY_DOWN);
                    windowData.eventHandler(event);
                    break;
                }
                case GLFW_RELEASE:
                {
                    KeyPressEvent event(Event::TYPE::KEY_PRESS, key, KeyPressEvent::ACTION::KEY_UP);
                    windowData.eventHandler(event);
                    break;
                }
                case GLFW_REPEAT:
                {
                    KeyPressEvent event(Event::TYPE::KEY_PRESS, key, KeyPressEvent::ACTION::KEY_HOLD);
                    windowData.eventHandler(event);
                    break;
                }
                }
            });

        // Bind mouse button event callback
        glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, int button, int action, int mods)
            {
                WindowData& windowData = *(WindowData*)glfwGetWindowUserPointer(window);
                if (!windowData.eventHandler) return;

                switch (action)
                {
                case GLFW_PRESS:
                {
                    MousePressEvent event(Event::TYPE::MOUSE_PRESS, button, MousePressEvent::ACTION::MOUSE_DOWN);
                    windowData.eventHandler(event);
                    break;
                }
                case GLFW_RELEASE:
                {
                    MousePressEvent event(Event::TYPE::MOUSE_PRESS, button, MousePressEvent::ACTION::MOUSE_UP);
                    windowData.eventHandler(event);
                    break;
                }
                case GLFW_REPEAT:
                {
                    MousePressEvent event(Event::TYPE::MOUSE_PRESS, button, MousePressEvent::ACTION::MOUSE_HOLD);
                    windowData.eventHandler(event);
                    break;
                }
                }
            });

        // Bind mouse scroll event callback
        glfwSetScrollCallback(m_window, [](GLFWwindow* window, double xOffset, double yOffset)
            {
                WindowData& windowData = *(WindowData*)glfwGetWindowUserPointer(window);
                if (!windowData.eventHandler) return;

                MouseWheelEvent event(Event::TYPE::MOUSE_WHEEL, (float)xOffset, (float)yOffset);
                windowData.eventHandler(event);
            });

        // Bind mouse cursor position event callback
        glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double xPos, double yPos)
            {
                WindowData& windowData = *(WindowData*)glfwGetWindowUserPointer(window);
                if (!windowData.eventHandler) return;

                MouseMoveEvent event(Event::TYPE::MOUSE_MOVE, (float)xPos, (float)yPos);
                windowData.eventHandler(event);
            });

        // Bind framebuffer resize event callback
        glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow* window, int width, int height)
            {
                WindowData& windowData = *(WindowData*)glfwGetWindowUserPointer(window);
                windowData.framebufferResized = true;
            });
    }

    void Window::Destroy() {
        glfwDestroyWindow(m_window);
        glfwTerminate();
    }

    void Window::Begin() {
        // Poll and handle events (inputs, window resize, etc.)
        glfwPollEvents();
    }

    void Window::End() {

    }
}
