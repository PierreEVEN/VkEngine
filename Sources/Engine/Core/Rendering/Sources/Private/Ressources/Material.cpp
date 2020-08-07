#include "Ressources/Material.h"


#include "Vulkan/VulkanLogicalDevice.h"
#include "Vulkan/VulkanUniformBuffer.h"
#include "RenderingTypes.h"
#include "Vulkan/VulkanAntialiasing.h"
#include "Vulkan/VulkanRenderPass.h"
#include "Vulkan/VulkanShaderModule.h"

#include <fstream>
#include "Vulkan/VulkanImage.h"


std::vector<char> ReadFile(const String& filename) {
	std::ifstream file(filename.GetData(), std::ios::ate | std::ios::binary);

	if (!file.is_open()) {
		LOG_ASSERT(String("Failed to open shader file ") + filename);
	}
	size_t fileSize = (size_t)file.tellg();
	std::vector<char> buffer(fileSize);
	file.seekg(0);
	file.read(buffer.data(), fileSize);
	file.close();

	return buffer;
}

void Rendering::Material::CreateOrUpdatePipeline()
{

	auto vertShaderCode = ReadFile("Shaders/vert.spv");
	auto fragShaderCode = ReadFile("Shaders/frag.spv");

	Vulkan::GraphicPipeline::ShaderModule vertShaderModule(vertShaderCode);
	Vulkan::GraphicPipeline::ShaderModule fragShaderModule(fragShaderCode);

	VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
	vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;

	vertShaderStageInfo.module = vertShaderModule.Get();
	vertShaderStageInfo.pName = "main";

	VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
	fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	fragShaderStageInfo.module = fragShaderModule.Get();
	fragShaderStageInfo.pName = "main";

	VkPipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo, fragShaderStageInfo };

	auto bindingDescription = Rendering::Vertex::GetBindingDescription();
	auto attributeDescriptions = Rendering::Vertex::GetAttributeDescriptions();

	VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
	vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertexInputInfo.vertexBindingDescriptionCount = 1;
	vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
	vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
	vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

	VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
	inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	inputAssembly.primitiveRestartEnable = VK_FALSE;

	VkViewport viewport{};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = static_cast<float>(G_SWAPCHAIN_EXTEND.width);
	viewport.height = static_cast<float>(G_SWAPCHAIN_EXTEND.height);
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;

	VkRect2D scissor{};
	scissor.offset = { 0, 0 };
	scissor.extent = G_SWAPCHAIN_EXTEND;

	VkPipelineViewportStateCreateInfo viewportState{};
	viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportState.viewportCount = 1;
	viewportState.pViewports = &viewport;
	viewportState.scissorCount = 1;
	viewportState.pScissors = &scissor;

	VkPipelineRasterizationStateCreateInfo rasterizer{};
	rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizer.depthClampEnable = VK_FALSE;
	rasterizer.rasterizerDiscardEnable = VK_FALSE;
	rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
	rasterizer.lineWidth = 1.0f;
	rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
	rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
	rasterizer.depthBiasEnable = VK_FALSE;
	rasterizer.depthBiasConstantFactor = 0.0f; // Optional
	rasterizer.depthBiasClamp = 0.0f; // Optional
	rasterizer.depthBiasSlopeFactor = 0.0f; // Optional

	VkPipelineMultisampleStateCreateInfo multisampling{};
	multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisampling.sampleShadingEnable = VK_FALSE;
	multisampling.rasterizationSamples = Vulkan::Antialiasing::GetMsaaSampleCount();
	multisampling.minSampleShading = 1.0f; // Optional
	multisampling.pSampleMask = nullptr; // Optional
	multisampling.alphaToCoverageEnable = VK_FALSE; // Optional
	multisampling.alphaToOneEnable = VK_FALSE; // Optional
	multisampling.sampleShadingEnable = VK_TRUE;
	multisampling.minSampleShading = .2f;

	VkPipelineDepthStencilStateCreateInfo depthStencil{};
	depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	depthStencil.depthTestEnable = VK_TRUE;
	depthStencil.depthWriteEnable = VK_TRUE;
	depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
	depthStencil.depthBoundsTestEnable = VK_FALSE;
	depthStencil.minDepthBounds = 0.0f; // Optional
	depthStencil.maxDepthBounds = 1.0f; // Optional
	depthStencil.stencilTestEnable = VK_FALSE;

	VkPipelineColorBlendAttachmentState colorBlendAttachment{};
	colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	colorBlendAttachment.blendEnable = VK_FALSE;
	colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
	colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
	colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD; // Optional
	colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
	colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
	colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD; // Optional

	VkPipelineColorBlendStateCreateInfo colorBlending{};
	colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	colorBlending.logicOpEnable = VK_FALSE;
	colorBlending.logicOp = VK_LOGIC_OP_COPY; // Optional
	colorBlending.attachmentCount = 1;
	colorBlending.pAttachments = &colorBlendAttachment;
	colorBlending.blendConstants[0] = 0.0f; // Optional
	colorBlending.blendConstants[1] = 0.0f; // Optional
	colorBlending.blendConstants[2] = 0.0f; // Optional
	colorBlending.blendConstants[3] = 0.0f; // Optional

	VkDynamicState dynamicStates[] = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_LINE_WIDTH };

	VkPipelineDynamicStateCreateInfo dynamicState{};
	dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	dynamicState.dynamicStateCount = 2;
	dynamicState.pDynamicStates = dynamicStates;

	VkGraphicsPipelineCreateInfo pipelineInfo{};
	pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipelineInfo.stageCount = 2;
	pipelineInfo.pStages = shaderStages;
	pipelineInfo.pVertexInputState = &vertexInputInfo;
	pipelineInfo.pInputAssemblyState = &inputAssembly;
	pipelineInfo.pViewportState = &viewportState;
	pipelineInfo.pRasterizationState = &rasterizer;
	pipelineInfo.pMultisampleState = &multisampling;
	pipelineInfo.pDepthStencilState = nullptr; // Optional
	pipelineInfo.pColorBlendState = &colorBlending;
	pipelineInfo.pDynamicState = nullptr; // Optional
	pipelineInfo.layout = G_PIPELINE_LAYOUT;
	pipelineInfo.renderPass = Vulkan::RenderPass::GetRenderPass();
	pipelineInfo.subpass = 0;
	pipelineInfo.basePipelineHandle = VK_NULL_HANDLE; // Optional
	pipelineInfo.basePipelineIndex = -1; // Optional
	pipelineInfo.pDepthStencilState = &depthStencil;

	if (vkCreateGraphicsPipelines(G_LOGICAL_DEVICE, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &shaderPipeline) != VK_SUCCESS) {
		LOG_ASSERT("Failed to create material graphic pipeline");
	}
}

void Rendering::Material::CreateOrUpdateDescriptorSets()
{
	VkDescriptorSetAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = G_DESCRIPTOR_POOL;
	allocInfo.descriptorSetCount = 1;
	allocInfo.pSetLayouts = &G_DESCRIPTOR_SET_LAYOUT;

	if (vkAllocateDescriptorSets(G_LOGICAL_DEVICE, &allocInfo, &descriptorSet) != VK_SUCCESS) {
		LOG_ASSERT("Failed to allocate descriptor sets");
	}

	VkDescriptorBufferInfo bufferInfo{};
	bufferInfo.buffer = Vulkan::UniformBuffer::GetUniformBuffer();
	bufferInfo.offset = 0;
	bufferInfo.range = sizeof(Vulkan::UniformBuffer::UniformBufferObject);

	VkDescriptorImageInfo imageInfo{};
	imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	imageInfo.imageView = Vulkan::Image::GetTextureImageView();
	imageInfo.sampler = Vulkan::Image::GetTextureSampler();

	std::array<VkWriteDescriptorSet, 2> descriptorWrites{};

	descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	descriptorWrites[0].dstSet = descriptorSet;
	descriptorWrites[0].dstBinding = 0;
	descriptorWrites[0].dstArrayElement = 0;
	descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	descriptorWrites[0].descriptorCount = 1;
	descriptorWrites[0].pBufferInfo = &bufferInfo;

	descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	descriptorWrites[1].dstSet = descriptorSet;
	descriptorWrites[1].dstBinding = 1;
	descriptorWrites[1].dstArrayElement = 0;
	descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	descriptorWrites[1].descriptorCount = 1;
	descriptorWrites[1].pImageInfo = &imageInfo;

	vkUpdateDescriptorSets(G_LOGICAL_DEVICE, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
}


void Rendering::Material::DestroyShaderPipeline()
{
	vkDestroyPipeline(G_LOGICAL_DEVICE, shaderPipeline, G_ALLOCATION_CALLBACK);
}

void Rendering::CreateDescriptorPool()
{

}

void Rendering::CreateOrUpdatePipelineLayout()
{
	if (G_PIPELINE_LAYOUT != VK_NULL_HANDLE)
		DestroyPipelineLayout();

	LOG("Create Graphic pipeline");
	VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = 1;            // Optional
	pipelineLayoutInfo.pSetLayouts = &Rendering::Vulkan::UniformBuffer::GetDescriptorSetLayout();
	pipelineLayoutInfo.pPushConstantRanges = nullptr; // Optional

	if (vkCreatePipelineLayout(Rendering::Vulkan::LogDevice::GetLogicalDevice(), &pipelineLayoutInfo, nullptr, &G_PIPELINE_LAYOUT) != VK_SUCCESS) {
		LOG_ASSERT("Failed to create pipeline layout");
	}
}

void Rendering::DestroyPipelineLayout()
{
	vkDestroyPipelineLayout(Rendering::Vulkan::LogDevice::GetLogicalDevice(), G_PIPELINE_LAYOUT, nullptr);
	G_PIPELINE_LAYOUT = VK_NULL_HANDLE;
}

void Rendering::DestroyDescriptorPool()
{
	LOG("Destroy Descriptor pool");
	vkDestroyDescriptorPool(G_LOGICAL_DEVICE, G_DESCRIPTOR_POOL, G_ALLOCATION_CALLBACK);
}

Rendering::Material::Material()
{
	CreateOrUpdateDescriptorSets();
	CreateOrUpdatePipeline();
}

Rendering::Material::~Material()
{
	DestroyShaderPipeline();
}

void Rendering::Material::Use(VkCommandBuffer commandBuffer)
{
	vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, G_PIPELINE_LAYOUT, 0, 1, &descriptorSet, 0, nullptr);
	vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, shaderPipeline);
}
