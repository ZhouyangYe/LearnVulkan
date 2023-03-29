#include "Descriptor.h"

namespace LearnVulkan {
	std::vector<VkDescriptorPool> Descriptor::descriptorPools;
	VkDescriptorPool Descriptor::_descriptorPool;

	Descriptor::Descriptor(Device* device) : device(device) {}

	Descriptor::~Descriptor() {}

	void Descriptor::init_uniforms(uint32_t frameNum, uint64_t camera_size, uint64_t scene_size)
	{
		alignedCameraDataSize = device->pad_uniform_buffer_size(camera_size);
		alignedSceneDataSize = device->pad_uniform_buffer_size(scene_size);

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

	Descriptor& Descriptor::add_texture_layout()
	{
		//another set, one that holds a single texture
		VkDescriptorSetLayoutBinding textureBind = vkinit::descriptorset_layout_binding(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT, 0);

		VkDescriptorSetLayoutCreateInfo setinfo = {};
		setinfo.bindingCount = 1;
		setinfo.flags = 0;
		setinfo.pNext = nullptr;
		setinfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		setinfo.pBindings = &textureBind;

		VkDescriptorSetLayout _textureLayout;
		vkCreateDescriptorSetLayout(device->_device, &setinfo, nullptr, &_textureLayout);

		_textureLayouts.push_back(_textureLayout);

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

	Descriptor& Descriptor::add_texture_descriptor_set(VkImageView imageView)
	{
		//create a sampler for the texture
		VkSamplerCreateInfo samplerInfo = vkinit::sampler_create_info(VK_FILTER_NEAREST);

		VkSampler sampler;
		vkCreateSampler(device->_device, &samplerInfo, nullptr, &sampler);

		VkDescriptorSet textureSet;

		//allocate the descriptor set for single-texture to use on the material
		VkDescriptorSetAllocateInfo allocInfo = {};
		allocInfo.pNext = nullptr;
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorPool = _descriptorPool;
		allocInfo.descriptorSetCount = 1;
		allocInfo.pSetLayouts = &_textureLayouts.back();

		vkAllocateDescriptorSets(device->_device, &allocInfo, &textureSet);

		//write to the descriptor set so that it points to our empire_diffuse texture
		VkDescriptorImageInfo imageBufferInfo;
		imageBufferInfo.sampler = sampler;
		imageBufferInfo.imageView = imageView;
		imageBufferInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

		VkWriteDescriptorSet textureWrite = vkinit::write_descriptor_image(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, textureSet, &imageBufferInfo, 0);

		vkUpdateDescriptorSets(device->_device, 1, &textureWrite, 0, nullptr);

		_samplers.push_back(sampler);
		_textureSets.push_back(textureSet);

		return *this;
	}

	void Descriptor::add_description_pool()
	{
		// create a descriptor pool that will hold 10 uniform buffers
		std::vector<VkDescriptorPoolSize> sizes =
		{
			{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 10 },
			//add combined-image-sampler descriptor types to the pool
			{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 10 }
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

		for (auto iter = _textureLayouts.begin(); iter != _textureLayouts.end(); ++iter) {
			vkDestroyDescriptorSetLayout(device->_device, *iter, nullptr);
		}

		for (auto iter = _samplers.begin(); iter != _samplers.end(); ++iter) {
			vkDestroySampler(device->_device, *iter, nullptr);
		}

		vmaDestroyBuffer(device->_allocator, _defaultUniformBuffer._buffer, _defaultUniformBuffer._allocation);
	}
}
