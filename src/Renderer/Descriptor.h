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
		Buffer _defaultUniformBuffer;
		std::vector<VkDescriptorSet> _descriptorSets;
		std::vector<VkDescriptorSetLayout> _layouts;

		uint64_t alignedCameraDataSize;
		uint64_t cameraParamBufferSize;

		uint64_t alignedSceneDataSize;
		uint64_t sceneParamBufferSize;

		static std::vector<VkDescriptorPool> descriptorPools;

		void init_uniforms(uint32_t frameNum, uint64_t camera_size, uint64_t scene_size);
		void update_uniform_data(GPUData& camera_uniform, GPUData& scene_uniform);
		Descriptor& add_uniform_layout();
		Descriptor& add_texture_layout();
		Descriptor& add_uniform_descriptor_set(uint32_t frameNum, uint64_t camera_size, uint64_t scene_size);

		void add_description_pool();

		void Destroy();
	private:
		Device* device;

		static VkDescriptorPool _descriptorPool;
	};
}
