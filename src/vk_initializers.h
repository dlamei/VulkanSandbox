#pragma once

#include "vk_types.h"

namespace vkinit {

struct fVec2D {
	float x, y;
};

struct uVec2D {
	uint32_t x, y;
};

VkCommandPoolCreateInfo
command_pool_create_info(uint32_t queueFamilyIndex,
                         VkCommandPoolCreateFlags flags = 0);

VkCommandBufferAllocateInfo command_buffer_allocate_info(
    VkCommandPool pool, uint32_t count = 1,
    VkCommandBufferLevel level = VK_COMMAND_BUFFER_LEVEL_PRIMARY);

VkCommandBufferBeginInfo
command_buffer_begin_info(VkCommandBufferUsageFlags flags = 0);

VkFramebufferCreateInfo framebuffer_create_info(VkRenderPass renderPass,
                                                VkExtent2D extent);

VkFenceCreateInfo fence_create_info(VkFenceCreateFlags flags = 0);

VkSemaphoreCreateInfo semaphore_create_info(VkSemaphoreCreateFlags flags = 0);

VkSubmitInfo submit_info(VkCommandBuffer *cmd);

VkPresentInfoKHR present_info();

VkRenderPassBeginInfo renderpass_begin_info(VkRenderPass renderPass,
                                            VkExtent2D windowExtent,
                                            VkFramebuffer framebuffer);

VkPipelineShaderStageCreateInfo
pipeline_shader_stage_create_info(VkShaderStageFlagBits stage,
                                  VkShaderModule shaderModule);

VkPipelineVertexInputStateCreateInfo vertex_input_state_create_info();

VkPipelineInputAssemblyStateCreateInfo
input_assembly_create_info(VkPrimitiveTopology topology);

VkPipelineRasterizationStateCreateInfo
rasterization_state_create_info(VkPolygonMode polygonMode);

VkPipelineMultisampleStateCreateInfo multisampling_state_create_info();

VkPipelineColorBlendAttachmentState color_blend_attachment_state();

VkPipelineLayoutCreateInfo pipeline_layout_create_info();

struct PipelineBuilder {
  std::vector<VkPipelineShaderStageCreateInfo> shaderStages;
  VkPipelineVertexInputStateCreateInfo vertexInputInfo;
  VkPipelineInputAssemblyStateCreateInfo inputAssembly;
  VkViewport viewport = {};
  VkRect2D scissor = {};
  VkPipelineRasterizationStateCreateInfo rasterizer;
  VkPipelineColorBlendAttachmentState colorBlendAttachment;
  VkPipelineMultisampleStateCreateInfo multisampling;
  VkPipelineLayout pipelineLayout = VK_NULL_HANDLE;

  VkDevice device = VK_NULL_HANDLE;
  VkRenderPass renderPass = VK_NULL_HANDLE;

  PipelineBuilder()
      : multisampling(multisampling_state_create_info()),
        colorBlendAttachment(color_blend_attachment_state()),
        rasterizer(rasterization_state_create_info(VK_POLYGON_MODE_FILL)),
        inputAssembly(
            input_assembly_create_info(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST)),
        vertexInputInfo(vertex_input_state_create_info()) {}

  PipelineBuilder &add_shader_module(VkShaderModule shaderModule,
                                     utils::ShaderType shaderType);
  PipelineBuilder &set_device(VkDevice device);
  PipelineBuilder &set_render_pass(VkRenderPass renderPass);
  PipelineBuilder &set_viewport(VkViewport viewport);
  PipelineBuilder &set_viewport(fVec2D zero, fVec2D size, fVec2D depth);
  PipelineBuilder &set_scissor(VkRect2D scissor);
  PipelineBuilder &set_scissor(VkOffset2D offset, VkExtent2D extent);
  PipelineBuilder &set_pipeline_layout(VkPipelineLayout layout);
  PipelineBuilder &set_vertex_description(
      VkVertexInputAttributeDescription *pAttributes, uint32_t attributesCount,
      VkVertexInputBindingDescription *pBindings, uint32_t bindingCount);

	std::optional<VkPipeline> build();
};

} // namespace vkinit
