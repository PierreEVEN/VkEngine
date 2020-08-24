#include "Ressources/MaterialRessource.h"
#include "Scene/Scene.h"
#include "Scene/VkSceneElements/MatrixUniformBuffers.h"
#include "Ressources/TextureRessource.h"
#include "DescriptorPool.h"
#include "Assets/Texture2D.h"

Rendering::MaterialRessource* G_LAST_USED_MATERIAL_RESSOURCE;
Rendering::MaterialRessourceItem* G_LAST_USED_MATERIAL_RESSOURCE_ITEM;

Rendering::MaterialRessource::MaterialRessource(const SMaterialStaticProperties& materialProperties) : Ressource()
{
	CreateDescriptorSets(MakeLayoutBindings(materialProperties));
	CreatePipeline(materialProperties);
}

Rendering::MaterialRessource::~MaterialRessource()
{
	DestroyShadersObjects();
}

void Rendering::MaterialRessource::Use(VkCommandBuffer commandBuffer, const size_t& imageIndex)
{
	if (G_LAST_USED_MATERIAL_RESSOURCE == this && lastDrawIamgeIndex == imageIndex) return;

	lastDrawIamgeIndex = imageIndex;
	G_LAST_USED_MATERIAL_RESSOURCE = this;

	vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &descriptorSets[imageIndex], 0, nullptr);
	vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, shaderPipeline);
}

std::vector<VkDescriptorSetLayoutBinding> Rendering::MaterialRessource::MakeLayoutBindings(const SMaterialStaticProperties& materialProperties)
{
	uint32_t currentId = 0;
	std::vector<VkDescriptorSetLayoutBinding> outBindings;

	if (materialProperties.bUseGlobalUbo) {
		VkDescriptorSetLayoutBinding uboLayoutBinding{};
		uboLayoutBinding.binding = currentId;
		uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		uboLayoutBinding.descriptorCount = 1;
		uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
		uboLayoutBinding.pImmutableSamplers = nullptr;
		outBindings.push_back(uboLayoutBinding);
		currentId++;
	}

	/** Vertex textures */
	for (uint32_t i = 0; i < materialProperties.VertexTexture2DCount; ++i) {
		VkDescriptorSetLayoutBinding samplerLayoutBinding{};
		samplerLayoutBinding.binding = currentId;
		samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		samplerLayoutBinding.descriptorCount = 1;
		samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
		samplerLayoutBinding.pImmutableSamplers = nullptr;
		outBindings.push_back(samplerLayoutBinding);
		currentId++;
	}

	/** Fragment textures */
	for (uint32_t i = 0; i < materialProperties.FragmentTexture2DCount; ++i) {
		VkDescriptorSetLayoutBinding samplerLayoutBinding{};
		samplerLayoutBinding.binding = currentId;
		samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		samplerLayoutBinding.descriptorCount = 1;
		samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
		samplerLayoutBinding.pImmutableSamplers = nullptr;
		outBindings.push_back(samplerLayoutBinding);
		currentId++;
	}
	return outBindings;
}

void Rendering::MaterialRessource::CreatePipeline(const SMaterialStaticProperties& materialProperties)
{
	VK_CHECK(descriptorSetLayout, "Descriptor set layout should be initialized before graphic pipeline");
	VK_CHECK(G_RENDER_PASS, "Render pass should be initialized before graphic pipeline");

	/** Shader pipeline */
	VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
	vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
	vertShaderStageInfo.module = materialProperties.vertexShaderModule->GetShaderModule();
	vertShaderStageInfo.pName = "main";

	VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
	fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	fragShaderStageInfo.module = materialProperties.fragmentShaderModule->GetShaderModule();
	fragShaderStageInfo.pName = "main";

	/** Model transform */
	VkPushConstantRange pushConstantRange;
	pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
	pushConstantRange.offset = 0;
	pushConstantRange.size = sizeof(Mat4f);

	/** Pipeline layout */
	VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = 1;
	pipelineLayoutInfo.pSetLayouts = &descriptorSetLayout;
	pipelineLayoutInfo.pushConstantRangeCount = 1;
	pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;
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
	rasterizer.polygonMode = materialProperties.materialCreationFlag & EMATERIAL_CREATION_FLAG_WIREFRAME ? VK_POLYGON_MODE_LINE : VK_POLYGON_MODE_FILL;
	rasterizer.lineWidth = 1.0f;
	rasterizer.cullMode = materialProperties.materialCreationFlag & EMATERIAL_CREATION_FLAG_DOUBLESIDED ? VK_CULL_MODE_NONE : VK_CULL_MODE_FRONT_BIT;
	rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
	rasterizer.depthBiasEnable = VK_FALSE;
	rasterizer.depthBiasConstantFactor = 0.0f; // Optional
	rasterizer.depthBiasClamp = 0.0f; // Optional
	rasterizer.depthBiasSlopeFactor = 0.0f; // Optional

	VkPipelineMultisampleStateCreateInfo multisampling{};
	multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisampling.rasterizationSamples = G_ENABLE_MULTISAMPLING.GetValue() ? G_MSAA_SAMPLE_COUNT : VK_SAMPLE_COUNT_1_BIT;
	multisampling.minSampleShading = 1.0f; // Optional
	multisampling.pSampleMask = nullptr; // Optional
	multisampling.alphaToCoverageEnable = VK_FALSE; // Optional
	multisampling.alphaToOneEnable = VK_FALSE; // Optional
	multisampling.sampleShadingEnable = G_ENABLE_MULTISAMPLING.GetValue() ? VK_TRUE : VK_FALSE;
	multisampling.minSampleShading = .2f;

	VkPipelineDepthStencilStateCreateInfo depthStencil{};
	depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	depthStencil.depthTestEnable = materialProperties.materialCreationFlag & EMATERIAL_CREATION_FLAG_DISABLE_DEPTH_TEST ? VK_FALSE : VK_TRUE;
	depthStencil.depthWriteEnable = materialProperties.materialCreationFlag & EMATERIAL_CREATION_FLAG_DISABLE_DEPTH_TEST ? VK_FALSE : VK_TRUE;
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

void Rendering::MaterialRessourceItem::PreDraw(ViewportInstance* inViewport, const size_t& imageIndex)
{
	// @TODO not always update descriptor sets
	UpdateDescriptorSets(inViewport);
}

void Rendering::MaterialRessourceItem::Draw(VkCommandBuffer commandBuffer, ViewportInstance* drawViewport, const size_t& imageIndex)
{
	if (G_LAST_USED_MATERIAL_RESSOURCE_ITEM == this && lastDrawIamgeIndex == imageIndex) return;
	G_LAST_USED_MATERIAL_RESSOURCE_ITEM = this;
	lastDrawIamgeIndex = imageIndex;

	parent->Use(commandBuffer, imageIndex);
}

void Rendering::MaterialRessourceItem::UpdateDescriptorSets(ViewportInstance* drawViewport)
{
	if (dynamicMaterialProperties.vertexTextures2D.size() != staticMaterialProperties.VertexTexture2DCount) {
		LOG_ASSERT("Vertex texure number (" + ToString(dynamicMaterialProperties.vertexTextures2D.size()) + ") should be the same than material properties VertexTexture2DCount (" + ToString(staticMaterialProperties.VertexTexture2DCount) + ")");
	}

	if (dynamicMaterialProperties.fragmentTextures2D.size() != staticMaterialProperties.FragmentTexture2DCount) {
		LOG_ASSERT("Fragment texure number (" + ToString(dynamicMaterialProperties.fragmentTextures2D.size()) + ") should be the same than material properties FragmentTexture2DCount (" + ToString(staticMaterialProperties.FragmentTexture2DCount) + ")");
	}

	for (int iIndex = 0; iIndex < G_SWAP_CHAIN_IMAGE_COUNT; ++iIndex) {
		std::vector<VkWriteDescriptorSet> newDescSets;
		uint32_t currentBinding = 0;

		/** Global UBO */
		if (staticMaterialProperties.bUseGlobalUbo) {

			VkWriteDescriptorSet matrixUbo{};
			matrixUbo.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			matrixUbo.dstSet = parent->GetDescriptorSet(iIndex);
			matrixUbo.dstBinding = currentBinding;
			matrixUbo.dstArrayElement = 0;
			matrixUbo.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			matrixUbo.descriptorCount = 1;
			matrixUbo.pBufferInfo = &drawViewport->GetViewportUbos()->GetDescriptorBufferInfo(iIndex);
			matrixUbo.pImageInfo = nullptr;
			matrixUbo.pNext = nullptr;
			newDescSets.push_back(matrixUbo);
			currentBinding++;
		}

		/** Vertex textures2D */
		std::vector<VkDescriptorImageInfo> vertexImageInfos(staticMaterialProperties.VertexTexture2DCount);
		for (uint32_t i = 0; i < staticMaterialProperties.VertexTexture2DCount; ++i) {
			vertexImageInfos[i].imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			vertexImageInfos[i].imageView = dynamicMaterialProperties.vertexTextures2D[i]->GetImageView();
			vertexImageInfos[i].sampler = dynamicMaterialProperties.vertexTextures2D[i]->GetSampler();

			VkWriteDescriptorSet imgWDescSet{};
			imgWDescSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			imgWDescSet.dstSet = parent->GetDescriptorSet(iIndex);
			imgWDescSet.dstBinding = currentBinding;
			imgWDescSet.dstArrayElement = 0;
			imgWDescSet.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			imgWDescSet.descriptorCount = 1;
			imgWDescSet.pImageInfo = &vertexImageInfos[i];
			imgWDescSet.pBufferInfo = nullptr;
			imgWDescSet.pNext = nullptr;
			newDescSets.push_back(imgWDescSet);
			currentBinding++;
		}

		/** Vertex textures2D */
		std::vector<VkDescriptorImageInfo> fragmentImageInfos(staticMaterialProperties.FragmentTexture2DCount);
		for (uint32_t i = 0; i < staticMaterialProperties.FragmentTexture2DCount; ++i) {
			fragmentImageInfos[i].imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			fragmentImageInfos[i].imageView = dynamicMaterialProperties.fragmentTextures2D[i]->GetImageView();
			fragmentImageInfos[i].sampler = dynamicMaterialProperties.fragmentTextures2D[i]->GetSampler();

			VkWriteDescriptorSet imgWDescSet{};
			imgWDescSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			imgWDescSet.dstSet = parent->GetDescriptorSet(iIndex);
			imgWDescSet.dstBinding = currentBinding;
			imgWDescSet.dstArrayElement = 0;
			imgWDescSet.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			imgWDescSet.descriptorCount = 1;
			imgWDescSet.pImageInfo = &fragmentImageInfos[i];
			imgWDescSet.pBufferInfo = nullptr;
			imgWDescSet.pNext = nullptr;
			newDescSets.push_back(imgWDescSet);
			currentBinding++;
		}
		vkUpdateDescriptorSets(G_LOGICAL_DEVICE, static_cast<uint32_t>(newDescSets.size()), newDescSets.data(), 0, nullptr);
	}
}
