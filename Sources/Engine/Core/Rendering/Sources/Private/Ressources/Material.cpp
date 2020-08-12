#include "Ressources/Material.h"
#include "Viewport/ViewportInstance.h"
#include "Viewport/MatrixUniformBuffers.h"
#include "Ressources/Texture.h"
#include "DescriptorPool.h"

Rendering::MaterialRessource::MaterialRessource(
	const std::vector<char>& vertexShaderModule,
	const std::vector<char>& fragmentShaderModule,
	std::vector<VkDescriptorSetLayoutBinding> layoutBindings,
	EMaterialCreationFlags creationFlags)
{
 	CreateDescriptorSets(layoutBindings);
 	CreatePipeline(vertexShaderModule, fragmentShaderModule, creationFlags);
}

Rendering::MaterialRessource::~MaterialRessource()
{
	DestroyShadersObjects();
}

void Rendering::MaterialRessource::Use(VkCommandBuffer commandBuffer, const size_t& imageIndex)
{
	vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &descriptorSets[imageIndex], 0, nullptr);
	vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, shaderPipeline);
}

void Rendering::MaterialRessource::UpdateDescriptorSets(std::vector<VkWriteDescriptorSet> descriptorWrites)
{
	vkUpdateDescriptorSets(G_LOGICAL_DEVICE, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
}

void Rendering::MaterialRessource::CreatePipeline(const std::vector<char>& vertexShaderCode, const std::vector<char>& fragmentShaderCode, EMaterialCreationFlags creationFlags)
{
	VK_CHECK(descriptorSetLayout, "Descriptor set layout should be initialized before graphic pipeline");
	VK_CHECK(G_RENDER_PASS, "Render pass should be initialized before graphic pipeline");

	VkShaderModule vertexShaderModule = VK_NULL_HANDLE;
	VkShaderModule fragmentShaderModule = VK_NULL_HANDLE;
	VkShaderModuleCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;

	createInfo.codeSize = vertexShaderCode.size();
	createInfo.pCode = reinterpret_cast<const uint32_t*>(vertexShaderCode.data());
	VK_ENSURE(vkCreateShaderModule(G_LOGICAL_DEVICE, &createInfo, nullptr, &vertexShaderModule), "Failed to create vertex shader module");
	createInfo.codeSize = fragmentShaderCode.size();
	createInfo.pCode = reinterpret_cast<const uint32_t*>(fragmentShaderCode.data());
	VK_ENSURE(vkCreateShaderModule(G_LOGICAL_DEVICE, &createInfo, nullptr, &fragmentShaderModule), "Failed to create fragment shader module");

	/** Shader pipeline */
	VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
	vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
	vertShaderStageInfo.module = vertexShaderModule;
	vertShaderStageInfo.pName = "main";

	VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
	fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	fragShaderStageInfo.module = fragmentShaderModule;
	fragShaderStageInfo.pName = "main";

	VkPushConstantRange pushConstantRange;
	pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
	pushConstantRange.offset = 0;
	pushConstantRange.size = sizeof(glm::mat4);

	/** Pipeline layout */
	VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = 1;            // Optional
	pipelineLayoutInfo.pSetLayouts = &descriptorSetLayout;
	pipelineLayoutInfo.pushConstantRangeCount = 1;
	pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange; // Optional
	VK_ENSURE(vkCreatePipelineLayout(G_LOGICAL_DEVICE, &pipelineLayoutInfo, nullptr, &pipelineLayout), "Failed to create pipeline layout");

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

	VkPipelineViewportStateCreateInfo viewportState{};
	viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportState.viewportCount = 1;
	viewportState.scissorCount = 1;

	VkPipelineRasterizationStateCreateInfo rasterizer{};
	rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizer.depthClampEnable = VK_FALSE;
	rasterizer.rasterizerDiscardEnable = VK_FALSE;
	rasterizer.polygonMode = creationFlags & EMATERIAL_CREATION_FLAG_WIREFRAME ? VK_POLYGON_MODE_LINE : VK_POLYGON_MODE_FILL;
	rasterizer.lineWidth = 1.0f;
	rasterizer.cullMode = creationFlags & EMATERIAL_CREATION_FLAG_DOUBLESIDED ? VK_CULL_MODE_NONE : VK_CULL_MODE_FRONT_BIT;
	rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
	rasterizer.depthBiasEnable = VK_FALSE;
	rasterizer.depthBiasConstantFactor = 0.0f; // Optional
	rasterizer.depthBiasClamp = 0.0f; // Optional
	rasterizer.depthBiasSlopeFactor = 0.0f; // Optional

	VkPipelineMultisampleStateCreateInfo multisampling{};
	multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisampling.rasterizationSamples = G_ENABLE_MULTISAMPLING ? G_MSAA_SAMPLE_COUNT : VK_SAMPLE_COUNT_1_BIT;
	multisampling.minSampleShading = 1.0f; // Optional
	multisampling.pSampleMask = nullptr; // Optional
	multisampling.alphaToCoverageEnable = VK_FALSE; // Optional
	multisampling.alphaToOneEnable = VK_FALSE; // Optional
	multisampling.sampleShadingEnable = G_ENABLE_MULTISAMPLING ? VK_TRUE : VK_FALSE;
	multisampling.minSampleShading = .2f;

	VkPipelineDepthStencilStateCreateInfo depthStencil{};
	depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	depthStencil.depthTestEnable = creationFlags & EMATERIAL_CREATION_FLAG_DISABLE_DEPTH_TEST ? VK_FALSE : VK_TRUE;
	depthStencil.depthWriteEnable = creationFlags & EMATERIAL_CREATION_FLAG_DISABLE_DEPTH_TEST ? VK_FALSE : VK_TRUE;
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

	VkDynamicState dynamicStates[] = { VK_DYNAMIC_STATE_SCISSOR, VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_LINE_WIDTH };

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
	pipelineInfo.pDynamicState = &dynamicState; // Optional
	pipelineInfo.layout = pipelineLayout;
	pipelineInfo.renderPass = G_RENDER_PASS;
	pipelineInfo.subpass = 0;
	pipelineInfo.basePipelineHandle = VK_NULL_HANDLE; // Optional
	pipelineInfo.basePipelineIndex = -1; // Optional
	pipelineInfo.pDepthStencilState = &depthStencil;
	VK_ENSURE(vkCreateGraphicsPipelines(G_LOGICAL_DEVICE, VK_NULL_HANDLE, 1, &pipelineInfo, G_ALLOCATION_CALLBACK, &shaderPipeline), "Failed to create material graphic pipeline");

	vkDestroyShaderModule(G_LOGICAL_DEVICE, vertexShaderModule, G_ALLOCATION_CALLBACK);
	vkDestroyShaderModule(G_LOGICAL_DEVICE, fragmentShaderModule, G_ALLOCATION_CALLBACK);
}

void Rendering::MaterialRessource::CreateDescriptorSets(std::vector<VkDescriptorSetLayoutBinding> layoutBindings)
{
	/** Create descriptor set layout */
	VkDescriptorSetLayoutCreateInfo layoutInfo{};
	layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutInfo.bindingCount = static_cast<uint32_t>(layoutBindings.size());
	layoutInfo.pBindings = layoutBindings.data();
	VK_ENSURE(vkCreateDescriptorSetLayout(G_LOGICAL_DEVICE, &layoutInfo, G_ALLOCATION_CALLBACK, &descriptorSetLayout), "Failed to create descriptor set layout");

	/** Allocate descriptor set */
	std::vector<VkDescriptorSetLayout> layouts(G_SWAP_CHAIN_IMAGE_COUNT, descriptorSetLayout);
	descriptorSets.resize(G_SWAP_CHAIN_IMAGE_COUNT);
	VkDescriptorSetAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorSetCount = static_cast<uint32_t>(G_SWAP_CHAIN_IMAGE_COUNT);
	allocInfo.pSetLayouts = layouts.data();
	allocInfo.descriptorPool = VK_NULL_HANDLE;
	ReserveDescriptorPoolMemory(allocInfo);
	VK_ENSURE(vkAllocateDescriptorSets(G_LOGICAL_DEVICE, &allocInfo, descriptorSets.data()), "Failed to allocate descriptor sets");
}

void Rendering::MaterialRessource::DestroyShadersObjects()
{
	vkDestroyPipeline(G_LOGICAL_DEVICE, shaderPipeline, G_ALLOCATION_CALLBACK);
	vkDestroyPipelineLayout(G_LOGICAL_DEVICE, pipelineLayout, G_ALLOCATION_CALLBACK);
	vkDestroyDescriptorSetLayout(G_LOGICAL_DEVICE, descriptorSetLayout, G_ALLOCATION_CALLBACK);
}

Rendering::MaterialInstance::MaterialInstance(const std::vector<VkWriteDescriptorSet>& inDescriptorSetInfos, MaterialRessource* inMaterial)
	: DescriptorSetInfos(inDescriptorSetInfos), parent(inMaterial)
{

}

void Rendering::MaterialInstance::Use(VkCommandBuffer commandBuffer, ViewportInstance* writeViewport, const size_t& imageIndex, glm::mat4 objectTransform)
{
	VkDescriptorBufferInfo bufferInfo{};
	bufferInfo.buffer = writeViewport->GetViewportUbos()->GetBuffer(imageIndex);
	bufferInfo.offset = 0;
	bufferInfo.range = sizeof(MatrixUniformBufferObject);

	VkWriteDescriptorSet matrixUbo{};
	matrixUbo.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	matrixUbo.dstSet = parent->GetDescriptorSet(imageIndex);
	matrixUbo.dstBinding = 0;
	matrixUbo.dstArrayElement = 0;
	matrixUbo.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	matrixUbo.descriptorCount = 1;
	matrixUbo.pBufferInfo = &bufferInfo;
	matrixUbo.pImageInfo = nullptr;
	matrixUbo.pNext = nullptr;


	VkDescriptorImageInfo imageInfo{};
	imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	imageInfo.imageView = G_DEFAULT_TEXTURE->GetImageView();
	imageInfo.sampler = G_DEFAULT_TEXTURE->GetSampler();

	VkWriteDescriptorSet imgWDescSet{};
	imgWDescSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	imgWDescSet.dstSet = G_MATERIAL_OPAQUE->GetDescriptorSet(imageIndex);
	imgWDescSet.dstBinding = 1;
	imgWDescSet.dstArrayElement = 0;
	imgWDescSet.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	imgWDescSet.descriptorCount = 1;
	imgWDescSet.pImageInfo = &imageInfo;
	imgWDescSet.pBufferInfo = nullptr;
	imgWDescSet.pNext = nullptr;

	std::vector<VkWriteDescriptorSet> wDescSet = { matrixUbo, imgWDescSet };

	for (const auto& desc : DescriptorSetInfos)
	{
		wDescSet.push_back(desc);
	}

	parent->UpdateDescriptorSets(wDescSet);

	vkCmdPushConstants(commandBuffer, parent->GetPipelineLayout(), VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(glm::mat4), &objectTransform);
	parent->Use(commandBuffer, imageIndex);
}
