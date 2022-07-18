#pragma once
#include <vulkan/vulkan.h>
#include <VkBootstrap.h>
#include "Core/Common.h"
#include "Window/Window.h"
#include "Helper.h"

#define VK_CHECK(x)                                                    \
	do                                                                 \
	{                                                                  \
		VkResult err = x;                                              \
		if (err)                                                       \
		{                                                              \
			std::cout <<"Detected Vulkan error: " << err << std::endl; \
			abort();                                                   \
		}                                                              \
	} while (0)