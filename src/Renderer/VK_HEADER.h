#pragma once
#include <vulkan/vulkan.h>
#include <VkBootstrap.h>
#include <vk_mem_alloc.h>
#include "Core/Common.h"
#include "Window/Window.h"

#define VK_CHECK(x)                                                                 \
	do                                                                              \
	{                                                                               \
		VkResult err = x;                                                           \
		if (err)                                                                    \
		{                                                                           \
			Logger::console->error(fmt::format("Detected Vulkan error: {}", err));  \
			abort();                                                                \
		}                                                                           \
	} while (0)
