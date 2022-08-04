#include "Descriptor.h"

namespace LearnVulkan {
	std::vector<VkDescriptorPool> Descriptor::descriptorPools;
	VkDescriptorPool Descriptor::_descriptorPool;

	Descriptor::Descriptor(Device* device) : device(device) {}

	Descriptor::~Descriptor() {}

	void Descriptor::init_uniforms(uint32_t frameNum, uint64_t size)
	{
		uniform_buffers.resize(frameNum);
		uniform_descriptorSets.resize(frameNum);
		add_uniform_layout().add_uniform_descriptor_set(frameNum, size);
	}

	void Descriptor::update_uniform_data(uint32_t frameIndex, const void* data, uint64_t size)
	{
		device->upload_data(uniform_buffers[frameIndex][0], data, size);
	}

	Descriptor& Descriptor::add_uniform_layout()
	{
		if (!descriptorPools.size()) {
			add_description_pool();
		}

		// information about the binding.
		VkDescriptorSetLayoutBinding bufferBinding = {};
		bufferBinding.binding = 0;
		bufferBinding.descriptorCount = 1;
		// it's a uniform buffer binding
		bufferBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;

		// we use it from the vertex shader
		bufferBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

		VkDescriptorSetLayoutCreateInfo setinfo = {};
		setinfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		setinfo.pNext = nullptr;

		// we are going to have 1 binding
		setinfo.bindingCount = 1;
		// no flags
		setinfo.flags = 0;
		// point to the camera buffer binding
		setinfo.pBindings = &bufferBinding;

		VkDescriptorSetLayout layout;
		vkCreateDescriptorSetLayout(device->_device, &setinfo, nullptr, &layout);

		uniform_layouts.push_back(layout);

		return *this;
	}

	Descriptor& Descriptor::add_uniform_descriptor_set(uint32_t frameNum, uint64_t size)
	{
		VkDescriptorSetLayout _layout = uniform_layouts.back();

		for (uint32_t i = 0; i < frameNum; ++i) {
			uniform_buffers[i].push_back(device->create_buffer(size, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VMA_MEMORY_USAGE_CPU_TO_GPU));

			// allocate one descriptor set for each frame
			VkDescriptorSetAllocateInfo allocInfo = {};
			allocInfo.pNext = nullptr;
			allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
			// using the pool we just set
			allocInfo.descriptorPool = _descriptorPool;
			// only 1 descriptor
			allocInfo.descriptorSetCount = 1;
			// using the global data layout
			allocInfo.pSetLayouts = &_layout;

			VkDescriptorSet descriptorSet;
			vkAllocateDescriptorSets(device->_device, &allocInfo, &descriptorSet);

			uniform_descriptorSets[i].push_back(descriptorSet);

			// information about the buffer we want to point at in the descriptor
			VkDescriptorBufferInfo binfo;
			// it will be the camera buffer
			binfo.buffer = uniform_buffers[i].back()._buffer;
			// at 0 offset
			binfo.offset = 0;
			// of the size of a camera data struct
			binfo.range = size;

			VkWriteDescriptorSet setWrite = {};
			setWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			setWrite.pNext = nullptr;

			// we are going to write into binding number 0
			setWrite.dstBinding = 0;
			// of the global descriptor
			setWrite.dstSet = descriptorSet;

			setWrite.descriptorCount = 1;
			// and the type is uniform buffer
			setWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			setWrite.pBufferInfo = &binfo;

			vkUpdateDescriptorSets(device->_device, 1, &setWrite, 0, nullptr);
		}

		return *this;
	}

	void Descriptor::add_description_pool()
	{
		// create a descriptor pool that will hold 10 uniform buffers
		std::vector<VkDescriptorPoolSize> sizes =
		{
			{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 10 }
		};

		VkDescriptorPoolCreateInfo pool_info = {};
		pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		pool_info.flags = 0;
		pool_info.maxSets = 10;
		pool_info.poolSizeCount = (uint32_t)sizes.size();
		pool_info.pPoolSizes = sizes.data();

		vkCreateDescriptorPool(device->_device, &pool_info, nullptr, &_descriptorPool);

		descriptorPools.push_back(_descriptorPool);
	}

	void Descriptor::Destroy()
	{
		for (auto iter = descriptorPools.begin(); iter != descriptorPools.end(); ++iter) {
			vkDestroyDescriptorPool(device->_device, *iter, nullptr);
		}

		for (auto iter = uniform_layouts.begin(); iter != uniform_layouts.end(); ++iter) {
			vkDestroyDescriptorSetLayout(device->_device, *iter, nullptr);
		}

		for (auto iter = uniform_buffers.begin(); iter != uniform_buffers.end(); ++iter) {
			for (auto iter2 = (*iter).begin(); iter2 != (*iter).end(); ++iter2) {
				vmaDestroyBuffer(device->_allocator, iter2->_buffer, iter2->_allocation);
			}
		}
	}
}
