#pragma once
#include "Core/Common.h";
#include "Device.h"
#include "Buffer.h"

namespace LearnVulkan {
	class Descriptor {
	public:
		Descriptor(Device* device);
		~Descriptor();

		// default uniforms
		std::vector<std::vector<Buffer>> uniform_buffers;
		std::vector<std::vector<VkDescriptorSet>> uniform_descriptorSets;
		std::vector<VkDescriptorSetLayout> uniform_layouts;

		static std::vector<VkDescriptorPool> descriptorPools;

		void init_uniforms(uint32_t frameNum, uint64_t size);
		void update_uniform_data(uint32_t frameIndex, const void* data, uint64_t size);
		Descriptor& add_uniform_layout();
		Descriptor& add_uniform_descriptor_set(uint32_t frameNum, uint64_t size);

		void add_description_pool();

		void Destroy();
	private:
		Device* device;

		static VkDescriptorPool _descriptorPool;
	};
}
