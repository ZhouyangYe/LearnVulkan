#pragma once
#include "VK_HEADER.h"
#include "Renderer.h"
#include "Buffer.h"
#include "Events/Error.h"

namespace LearnVulkan {
	class Texture {
	public:
		ImageBuffer image;
		VkImageView imageView;

		VkDescriptorSet textureSet;

		Renderer* renderer;

		Texture(Renderer& renderer, const char* file);
		~Texture();

		void create_set();

		void Destroy();
	private:
		void load_image_from_file(const char* file);
	};
}
