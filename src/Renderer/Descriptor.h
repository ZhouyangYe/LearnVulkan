#pragma once
#include "Core/Common.h";
#include "Device.h"
#include "Buffer.h"
#include "Helper.h"

namespace LearnVulkan {
	class Descriptor {
	public:
		Descriptor(Device* device);
		~Descriptor();

		// default uniforms
		std::vector<Buffer> _cameraUniformBuffers;
		Buffer _sceneParameterBuffer;
		std::vector<VkDescriptorSet> _uniformDescriptorSets;
		std::vector<VkDescriptorSetLayout> uniform_layouts;

		static std::vector<VkDescriptorPool> descriptorPools;

		void init_uniforms(uint32_t frameNum, uint64_t vertex_size, uint64_t fragment_size);
		void update_uniform_data(uint32_t frameIndex, GPUData& vertex_uniform, GPUData& fragment_uniform);
		Descriptor& add_uniform_layout();
		Descriptor& add_uniform_descriptor_set(uint32_t frameNum, uint64_t vertex_size, uint64_t fragment_size);

		void add_description_pool();

		void Destroy();
	private:
		Device* device;

		static VkDescriptorPool _descriptorPool;
	};
}
