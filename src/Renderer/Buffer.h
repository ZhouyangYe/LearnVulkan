#pragma once
#include "VK_HEADER.h"

namespace LearnVulkan {
	class Buffer {
	public:
		VkBuffer _buffer;
		VmaAllocation _allocation;

		Buffer();
		~Buffer();
	};

	class ImageBuffer {
	public:
		VkImage _image;
		VmaAllocation _allocation;

		ImageBuffer();
		~ImageBuffer();
	};
}
