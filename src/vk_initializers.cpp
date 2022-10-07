#include "vk_initializers.h"

namespace vkinit {

VkCommandPoolCreateInfo
command_pool_create_info(uint32_t queueFamilyIndex,
                         VkCommandPoolCreateFlags flags) {

  VkCommandPoolCreateInfo info = {};
  info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
  info.pNext = nullptr;

  info.queueFamilyIndex = queueFamilyIndex;
  info.flags = flags;
  return info;
}

VkCommandBufferAllocateInfo
command_buffer_allocate_info(VkCommandPool pool, uint32_t count,
                             VkCommandBufferLevel level) {

  VkCommandBufferAllocateInfo info = {};
  info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  info.pNext = nullptr;

  info.commandPool = pool;
  info.commandBufferCount = count;
  info.level = level;
  return info;
}

VkCommandBufferBeginInfo
command_buffer_begin_info(VkCommandBufferUsageFlags flags) {
  VkCommandBufferBeginInfo info = {};
  info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
  info.pNext = nullptr;

  info.pInheritanceInfo = nullptr;
  info.flags = flags;
  return info;
}

VkFramebufferCreateInfo framebuffer_create_info(VkRenderPass renderPass,
                                                VkExtent2D extent) {
  VkFramebufferCreateInfo info = {};
  info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
  info.pNext = nullptr;

  info.renderPass = renderPass;
  info.attachmentCount = 1;
  info.width = extent.width;
  info.height = extent.height;
  info.layers = 1;

  return info;
}

VkFenceCreateInfo fence_create_info(VkFenceCreateFlags flags) {
  VkFenceCreateInfo info = {};
  info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
  info.pNext = nullptr;
  info.flags = flags;
  return info;
}

VkSemaphoreCreateInfo semaphore_create_info(VkSemaphoreCreateFlags flags) {
  VkSemaphoreCreateInfo info = {};
  info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
  info.pNext = nullptr;
  info.flags = flags;
  return info;
}

VkSubmitInfo submit_info(VkCommandBuffer *cmd) {
  VkSubmitInfo info = {};
  info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
  info.pNext = nullptr;

  info.waitSemaphoreCount = 0;
  info.pWaitSemaphores = nullptr;
  info.pWaitDstStageMask = nullptr;
  info.commandBufferCount = 1;
  info.pCommandBuffers = cmd;
  info.signalSemaphoreCount = 0;
  info.pSignalSemaphores = nullptr;

  return info;
}

VkPresentInfoKHR present_info() {
  VkPresentInfoKHR info = {};
  info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
  info.pNext = nullptr;

  info.swapchainCount = 0;
  info.pSwapchains = nullptr;
  info.pWaitSemaphores = nullptr;
  info.waitSemaphoreCount = 0;
  info.pImageIndices = nullptr;

  return info;
}

VkRenderPassBeginInfo renderpass_begin_info(VkRenderPass renderPass,
                                            VkExtent2D windowExtent,
                                            VkFramebuffer framebuffer) {
  VkRenderPassBeginInfo info = {};
  info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
  info.pNext = nullptr;

  info.renderPass = renderPass;
  info.renderArea.offset.x = 0;
  info.renderArea.offset.y = 0;
  info.renderArea.extent = windowExtent;
  info.clearValueCount = 1;
  info.pClearValues = nullptr;
  info.framebuffer = framebuffer;

  return info;
}

VkPipelineShaderStageCreateInfo
pipeline_shader_stage_create_info(VkShaderStageFlagBits stage,
                                  VkShaderModule shaderModule) {

  VkPipelineShaderStageCreateInfo info{};
  info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
  info.pNext = nullptr;

  info.stage = stage;
  info.module = shaderModule;
  info.pName = "main"; // entry point of the shader

  return info;
}

VkPipelineVertexInputStateCreateInfo vertex_input_state_create_info() {
  VkPipelineVertexInputStateCreateInfo info = {};
  info.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
  info.pNext = nullptr;

  info.vertexBindingDescriptionCount = 0;
  info.vertexAttributeDescriptionCount = 0;
  return info;
}

VkPipelineInputAssemblyStateCreateInfo
input_assembly_create_info(VkPrimitiveTopology topology) {
  VkPipelineInputAssemblyStateCreateInfo info = {};
  info.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
  info.pNext = nullptr;

  info.topology = topology;
  info.primitiveRestartEnable = VK_FALSE; // never used
  return info;
}

VkPipelineRasterizationStateCreateInfo
rasterization_state_create_info(VkPolygonMode polygonMode) {
  // TODO: check cull + depth settings
  VkPipelineRasterizationStateCreateInfo info = {};
  info.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
  info.pNext = nullptr;

  info.depthClampEnable = VK_FALSE;
  // discards all primitives before the rasterization stage if enabled which we
  // don't want
  info.rasterizerDiscardEnable = VK_FALSE;

  info.polygonMode = polygonMode;
  info.lineWidth = 1.0f;

  info.cullMode = VK_CULL_MODE_NONE;
  info.frontFace = VK_FRONT_FACE_CLOCKWISE;

  info.depthBiasEnable = VK_FALSE;
  info.depthBiasConstantFactor = 0.0f;
  info.depthBiasClamp = 0.0f;
  info.depthBiasSlopeFactor = 0.0f;

  return info;
}

VkPipelineMultisampleStateCreateInfo multisampling_state_create_info() {
  // TODO: to enable this set rasterizationSamples to more than 1
  //  has to be supported by renderpass
  VkPipelineMultisampleStateCreateInfo info = {};
  info.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
  info.pNext = nullptr;

  info.sampleShadingEnable = VK_FALSE;
  info.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
  info.minSampleShading = 1.0f;
  info.pSampleMask = nullptr;
  info.alphaToCoverageEnable = VK_FALSE;
  info.alphaToOneEnable = VK_FALSE;
  return info;
}

VkPipelineColorBlendAttachmentState color_blend_attachment_state() {
  VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
  colorBlendAttachment.colorWriteMask =
      VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
      VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
  colorBlendAttachment.blendEnable = VK_FALSE;
  return colorBlendAttachment;
}

VkPipelineLayoutCreateInfo pipeline_layout_create_info() {
  VkPipelineLayoutCreateInfo info{};
  info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
  info.pNext = nullptr;

  info.flags = 0;
  info.setLayoutCount = 0;
  info.pSetLayouts = nullptr;
  info.pushConstantRangeCount = 0;
  info.pPushConstantRanges = nullptr;
  return info;
}

PipelineBuilder &
PipelineBuilder::add_shader_module(VkShaderModule shaderModule,
                                   utils::ShaderType shaderType) {
  VkShaderStageFlagBits flag;

  switch (shaderType) {
  case utils::ShaderType::Vertex:
    flag = VK_SHADER_STAGE_VERTEX_BIT;
    break;
  case utils::ShaderType::Fragment:
    flag = VK_SHADER_STAGE_FRAGMENT_BIT;
    break;
  }

  shaderStages.push_back(pipeline_shader_stage_create_info(flag, shaderModule));
  return *this;
}
PipelineBuilder &PipelineBuilder::set_viewport(VkViewport viewport) {
  this->viewport = viewport;
  return *this;
}
PipelineBuilder &PipelineBuilder::set_scissor(VkRect2D scissor) {
  this->scissor = scissor;
  return *this;
}
PipelineBuilder &PipelineBuilder::set_scissor(VkOffset2D offset,
                                              VkExtent2D extent) {
  scissor.offset = offset;
  scissor.extent = extent;
  return *this;
}
PipelineBuilder &PipelineBuilder::set_pipeline_layout(VkPipelineLayout layout) {
  pipelineLayout = layout;
  return *this;
}
PipelineBuilder &PipelineBuilder::set_vertex_description(
    VkVertexInputAttributeDescription *pAttributes, uint32_t attributesCount,
    VkVertexInputBindingDescription *pBindings, uint32_t bindingCount) {
  vertexInputInfo.pVertexAttributeDescriptions = pAttributes;
  vertexInputInfo.vertexAttributeDescriptionCount = attributesCount;
  vertexInputInfo.pVertexBindingDescriptions = pBindings;
  vertexInputInfo.vertexBindingDescriptionCount = bindingCount;
  return *this;
}
PipelineBuilder &PipelineBuilder::set_render_pass(VkRenderPass renderPass) {
  this->renderPass = renderPass;
  return *this;
}
PipelineBuilder &PipelineBuilder::set_device(VkDevice device) {
  this->device = device;
  return *this;
}

std::optional<VkPipeline> PipelineBuilder::build() {

  if (device == VK_NULL_HANDLE) {
    CORE_ERROR("Could not build Pipeline: VkDevice is not set");
    return std::nullopt;
  } else if (renderPass == VK_NULL_HANDLE) {
    CORE_ERROR("Could not build Pipeline: VkRenderPass is not set");
    return std::nullopt;
  } else if (pipelineLayout == VK_NULL_HANDLE) {
    CORE_ERROR("Could not build Pipeline: VkPipelineLayout is not set");
    return std::nullopt;
  }

  VkPipelineViewportStateCreateInfo viewportState = {};
  viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
  viewportState.pNext = nullptr;

  viewportState.viewportCount = 1;
  viewportState.pViewports = &viewport;
  viewportState.scissorCount = 1;
  viewportState.pScissors = &scissor;

  VkPipelineColorBlendStateCreateInfo colorBlending = {};
  colorBlending.sType =
      VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
  colorBlending.pNext = nullptr;

  colorBlending.logicOpEnable = VK_FALSE;
  colorBlending.logicOp = VK_LOGIC_OP_COPY;
  colorBlending.attachmentCount = 1;
  colorBlending.pAttachments = &colorBlendAttachment;

  VkGraphicsPipelineCreateInfo pipelineInfo = {};
  pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
  pipelineInfo.pNext = nullptr;

  pipelineInfo.stageCount = shaderStages.size();
  pipelineInfo.pStages = shaderStages.data();
  pipelineInfo.pVertexInputState = &vertexInputInfo;
  pipelineInfo.pInputAssemblyState = &inputAssembly;
  pipelineInfo.pViewportState = &viewportState;
  pipelineInfo.pRasterizationState = &rasterizer;
  pipelineInfo.pMultisampleState = &multisampling;
  pipelineInfo.pColorBlendState = &colorBlending;
  pipelineInfo.layout = pipelineLayout;
  pipelineInfo.renderPass = renderPass;
  pipelineInfo.subpass = 0;
  pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

  VkPipeline pipeLine;

  if (vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineInfo,
                                nullptr, &pipeLine) != VK_SUCCESS) {
    CORE_ERROR("failed to create pipeline");
    return VK_NULL_HANDLE;
  } else {
    return pipeLine;
  }
}

PipelineBuilder &PipelineBuilder::set_viewport(fVec2D zero, fVec2D size,
                                               fVec2D depth) {
  viewport.x = zero.x;
  viewport.y = zero.y;
  viewport.width = size.x;
  viewport.height = size.y;
  viewport.minDepth = depth.x;
  viewport.maxDepth = depth.y;
  return *this;
}
PipelineBuilder &PipelineBuilder::set_vertex_description(
    std::vector<VkVertexInputAttributeDescription> &attributes,
    std::vector<VkVertexInputBindingDescription> &bindings) {
  vertexInputInfo.pVertexAttributeDescriptions = attributes.data();
  vertexInputInfo.vertexAttributeDescriptionCount = attributes.size();
  vertexInputInfo.pVertexBindingDescriptions = bindings.data();
  vertexInputInfo.vertexBindingDescriptionCount = bindings.size();
  return *this;
}
} // namespace vkinit
