#include "Descriptor.h"

namespace LearnVulkan {
	std::vector<VkDescriptorPool> Descriptor::descriptorPools;
	VkDescriptorPool Descriptor::_descriptorPool;

	Descriptor::Descriptor(Device* device) : device(device) {}

	Descriptor::~Descriptor() {}

	void Descriptor::init_uniforms(uint32_t frameNum, uint64_t camera_size, uint64_t scene_size)
	{
		alignedCameraDataSize = device->pad_uniform_buffer_size(sizeof(camera_size));
		alignedSceneDataSize = device->pad_uniform_buffer_size(sizeof(scene_size));
		add_uniform_layout().add_uniform_descriptor_set(frameNum, camera_size, scene_size);
	}

	void Descriptor::update_uniform_data(GPUData& camera_uniform, GPUData& scene_uniform)
	{
		device->upload_data(_defaultUniformBuffer, camera_uniform);

		scene_uniform.offset += cameraParamBufferSize;
		device->upload_data(_defaultUniformBuffer, scene_uniform);
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
		VkDescriptorSetLayoutBinding cameraBind = vkinit::descriptorset_layout_binding(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, VK_SHADER_STAGE_VERTEX_BIT, 0);

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

		_layouts.push_back(layout);

		return *this;
	}

	Descriptor& Descriptor::add_uniform_descriptor_set(uint32_t frameNum, uint64_t camera_size, uint64_t scene_size)
	{
		VkDescriptorSetLayout _layout = _layouts.back();

		// scene data buffer
		cameraParamBufferSize = frameNum * alignedCameraDataSize;
		sceneParamBufferSize = frameNum * alignedSceneDataSize;
		_defaultUniformBuffer = device->create_buffer(cameraParamBufferSize + sceneParamBufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VMA_MEMORY_USAGE_CPU_TO_GPU);

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

		_descriptorSets.push_back(descriptorSet);

		VkDescriptorBufferInfo cameraInfo;
		cameraInfo.buffer = _defaultUniformBuffer._buffer;
		cameraInfo.offset = 0;
		cameraInfo.range = camera_size;

		VkDescriptorBufferInfo sceneInfo;
		sceneInfo.buffer = _defaultUniformBuffer._buffer;
		sceneInfo.offset = cameraParamBufferSize;
		sceneInfo.range = scene_size;

		VkWriteDescriptorSet cameraWrite = vkinit::write_descriptor_buffer(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, descriptorSet, &cameraInfo, 0);

		VkWriteDescriptorSet sceneWrite = vkinit::write_descriptor_buffer(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, descriptorSet, &sceneInfo, 1);

		VkWriteDescriptorSet setWrites[] = { cameraWrite, sceneWrite };

		vkUpdateDescriptorSets(device->_device, 2, setWrites, 0, nullptr);

		return *this;
	}

	void Descriptor::add_description_pool()
	{
		// create a descriptor pool that will hold 10 uniform buffers
		std::vector<VkDescriptorPoolSize> sizes =
		{
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

		for (auto iter = _layouts.begin(); iter != _layouts.end(); ++iter) {
			vkDestroyDescriptorSetLayout(device->_device, *iter, nullptr);
		}

		vmaDestroyBuffer(device->_allocator, _defaultUniformBuffer._buffer, _defaultUniformBuffer._allocation);
	}
}
