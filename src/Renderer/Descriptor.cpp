#include "Descriptor.h"

namespace LearnVulkan {
	std::vector<VkDescriptorPool> Descriptor::descriptorPools;
	VkDescriptorPool Descriptor::_descriptorPool;

	Descriptor::Descriptor(Device* device) : device(device) {}

	Descriptor::~Descriptor() {}

	void Descriptor::init_uniforms(uint32_t frameNum, uint64_t vertex_size, uint64_t fragment_size)
	{
		_cameraUniformBuffers.reserve(frameNum);
		_uniformDescriptorSets.reserve(frameNum);
		add_uniform_layout().add_uniform_descriptor_set(frameNum, vertex_size, fragment_size);
	}

	void Descriptor::update_uniform_data(uint32_t frameIndex, GPUData& vertex_uniform, GPUData& fragment_uniform)
	{
		device->upload_data(_cameraUniformBuffers[frameIndex], vertex_uniform);

		device->upload_data(_sceneParameterBuffer, fragment_uniform);
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

		// binding for camera data at 0
		VkDescriptorSetLayoutBinding cameraBind = vkinit::descriptorset_layout_binding(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT, 0);

		// binding for scene data at 1
		VkDescriptorSetLayoutBinding sceneBind = vkinit::descriptorset_layout_binding(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 1);

		VkDescriptorSetLayoutBinding bindings[] = { cameraBind, sceneBind };

		VkDescriptorSetLayoutCreateInfo setinfo = {};
		setinfo.bindingCount = 2;
		setinfo.flags = 0;
		setinfo.pNext = nullptr;
		setinfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		setinfo.pBindings = bindings;

		VkDescriptorSetLayout layout;
		vkCreateDescriptorSetLayout(device->_device, &setinfo, nullptr, &layout);

		uniform_layouts.push_back(layout);

		return *this;
	}

	Descriptor& Descriptor::add_uniform_descriptor_set(uint32_t frameNum, uint64_t vertex_size, uint64_t fragment_size)
	{
		VkDescriptorSetLayout _layout = uniform_layouts.back();

		// scene data buffer
		const uint64_t sceneParamBufferSize = frameNum * device->pad_uniform_buffer_size(fragment_size);
		_sceneParameterBuffer = device->create_buffer(sceneParamBufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VMA_MEMORY_USAGE_CPU_TO_GPU);

		for (uint32_t i = 0; i < frameNum; ++i) {
			_cameraUniformBuffers.push_back(device->create_buffer(vertex_size, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VMA_MEMORY_USAGE_CPU_TO_GPU));

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

			_uniformDescriptorSets.push_back(descriptorSet);

			VkDescriptorBufferInfo cameraInfo;
			cameraInfo.buffer = _cameraUniformBuffers.back()._buffer;
			cameraInfo.offset = 0;
			cameraInfo.range = vertex_size;

			VkDescriptorBufferInfo sceneInfo;
			sceneInfo.buffer = _sceneParameterBuffer._buffer;
			sceneInfo.offset = 0;
			sceneInfo.range = fragment_size;

			VkWriteDescriptorSet cameraWrite = vkinit::write_descriptor_buffer(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, descriptorSet, &cameraInfo, 0);

			VkWriteDescriptorSet sceneWrite = vkinit::write_descriptor_buffer(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, descriptorSet, &sceneInfo, 1);

			VkWriteDescriptorSet setWrites[] = { cameraWrite, sceneWrite };

			vkUpdateDescriptorSets(device->_device, 2, setWrites, 0, nullptr);
		}

		return *this;
	}

	void Descriptor::add_description_pool()
	{
		// create a descriptor pool that will hold 10 uniform buffers
		std::vector<VkDescriptorPoolSize> sizes =
		{
			{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 10 },
			{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 10 }
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

		for (auto iter = _cameraUniformBuffers.begin(); iter != _cameraUniformBuffers.end(); ++iter) {
			vmaDestroyBuffer(device->_allocator, iter->_buffer, iter->_allocation);
		}

		vmaDestroyBuffer(device->_allocator, _sceneParameterBuffer._buffer, _sceneParameterBuffer._allocation);
	}
}
