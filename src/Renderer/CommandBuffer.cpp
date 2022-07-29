#include "CommandBuffer.h"

namespace LearnVulkan {
	CommandBuffer::CommandBuffer(Device* device)
		: device(device) {}

	CommandBuffer::~CommandBuffer() {}

	void CommandBuffer::Destroy()
	{
		for (auto iter = _commandPools.begin(); iter != _commandPools.end(); ++iter) {
			vkDestroyCommandPool(device->_device, *iter, nullptr);
		}

		vkDestroyRenderPass(device->_device, _renderPass, nullptr);
	}

	void CommandBuffer::init_commands(uint32_t num)
	{
		_commandBuffers.resize(num);
		_commandPools.resize(num);

		// create a command pool for commands submitted to the graphics queue.
		// we also want the pool to allow for resetting of individual command buffers
		VkCommandPoolCreateInfo commandPoolInfo = vkinit::command_pool_create_info(device->_graphicsQueueFamily, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);

		for (int i = 0; i < num; i++) {
			VK_CHECK(vkCreateCommandPool(device->_device, &commandPoolInfo, nullptr, &_commandPools[i]));

			//allocate the default command buffer that we will use for rendering
			VkCommandBufferAllocateInfo cmdAllocInfo = vkinit::command_buffer_allocate_info(_commandPools[i], 1);

			VK_CHECK(vkAllocateCommandBuffers(device->_device, &cmdAllocInfo, &_commandBuffers[i]));
		}
	}

	void CommandBuffer::init_renderpass(float clearColor[4], VkFormat swapchainImageFormat, VkFormat depthFormat)
	{
		this->clearColor = { clearColor[0], clearColor[1], clearColor[2], clearColor[3] };

		// we define an attachment description for our main color image
		// the attachment is loaded as "clear" when renderpass start
		// the attachment is stored when renderpass ends
		// the attachment layout starts as "undefined", and transitions to "Present" so its possible to display it
		// we dont care about stencil, and dont use multisampling

		VkAttachmentDescription color_attachment = {};
		color_attachment.format = swapchainImageFormat;
		color_attachment.samples = VK_SAMPLE_COUNT_1_BIT;
		color_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		color_attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		color_attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		color_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		color_attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		color_attachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;


		// VkAttachmentDescription color_attachment code
		VkAttachmentDescription depth_attachment = {};
		// Depth attachment
		depth_attachment.flags = 0;
		depth_attachment.format = depthFormat;
		depth_attachment.samples = VK_SAMPLE_COUNT_1_BIT;
		depth_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		depth_attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		depth_attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		depth_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		depth_attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		depth_attachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

		VkAttachmentReference depth_attachment_ref = {};
		depth_attachment_ref.attachment = 1;
		depth_attachment_ref.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

		VkAttachmentReference color_attachment_ref = {};
		color_attachment_ref.attachment = 0;
		color_attachment_ref.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		// we are going to create 1 subpass, which is the minimum you can do
		VkSubpassDescription subpass = {};
		subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpass.colorAttachmentCount = 1;
		subpass.pColorAttachments = &color_attachment_ref;
		// hook the depth attachment into the subpass
		subpass.pDepthStencilAttachment = &depth_attachment_ref;

		// 1 dependency, which is from "outside" into the subpass. And we can read or write color
		VkSubpassDependency dependency = {};
		dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
		dependency.dstSubpass = 0;
		dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.srcAccessMask = 0;
		dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

		VkSubpassDependency depth_dependency = {};
		depth_dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
		depth_dependency.dstSubpass = 0;
		depth_dependency.srcStageMask = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
		depth_dependency.srcAccessMask = 0;
		depth_dependency.dstStageMask = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
		depth_dependency.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

		VkSubpassDependency dependencies[2] = { dependency, depth_dependency };

		// array of 2 attachments, one for the color, and other for depth
		VkAttachmentDescription attachments[2] = { color_attachment,depth_attachment };

		VkRenderPassCreateInfo render_pass_info = {};
		render_pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		// 2 attachments from said array
		render_pass_info.attachmentCount = 2;
		render_pass_info.pAttachments = &attachments[0];
		render_pass_info.subpassCount = 1;
		render_pass_info.pSubpasses = &subpass;
		render_pass_info.dependencyCount = 2;
		render_pass_info.pDependencies = &dependencies[0];

		VK_CHECK(vkCreateRenderPass(device->_device, &render_pass_info, nullptr, &_renderPass));
	}

	void CommandBuffer::begin_command()
	{
		// now that we are sure that the commands finished executing, we can safely reset the command buffer to begin recording again.
		VK_CHECK(vkResetCommandBuffer(_mainCommandBuffer, 0));

		// begin the command buffer recording. We will use this command buffer exactly once, so we want to let vulkan know that
		VkCommandBufferBeginInfo cmdBeginInfo = vkinit::command_buffer_begin_info(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);

		VK_CHECK(vkBeginCommandBuffer(_mainCommandBuffer, &cmdBeginInfo));
	}

	void CommandBuffer::end_command()
	{
		// finalize the command buffer (we can no longer add commands, but it can now be executed)
		VK_CHECK(vkEndCommandBuffer(_mainCommandBuffer));
	}

	void CommandBuffer::begin_renderPass(VkSwapchainKHR swapChain, uint32_t swapchainImageIndex, std::vector<VkFramebuffer>& _framebuffers)
	{
		// make a clear-color from frame number. This will flash with a 120 frame period.
		VkClearValue clearValue;
		clearValue.color = clearColor;

		// clear depth at 1
		VkClearValue depthClear;
		depthClear.depthStencil.depth = 1.f;

		// start the main renderpass. 
		// We will use the clear color from above, and the framebuffer of the index the swapchain gave us
		VkRenderPassBeginInfo rpInfo = vkinit::renderpass_begin_info(_renderPass, device->_windowExtent, _framebuffers[swapchainImageIndex]);

		// connect clear values
		VkClearValue clearValues[] = { clearValue, depthClear };
		rpInfo.clearValueCount = 2;
		rpInfo.pClearValues = &clearValues[0];

		vkCmdBeginRenderPass(_mainCommandBuffer, &rpInfo, VK_SUBPASS_CONTENTS_INLINE);
	}

	void CommandBuffer::end_renderPass()
	{
		// finalize the render pass
		vkCmdEndRenderPass(_mainCommandBuffer);
	}

	void CommandBuffer::bind(VkPipeline& pipeline)
	{
		vkCmdBindPipeline(_mainCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
	}

	void CommandBuffer::setCommand(uint32_t index)
	{
		_mainCommandBuffer = _commandBuffers[index];
	}
}
