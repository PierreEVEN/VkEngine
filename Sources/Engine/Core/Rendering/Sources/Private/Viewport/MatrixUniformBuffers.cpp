#include "Viewport/MatrixUniformBuffers.h"
#include "Utils.h"
#include "Viewport/ViewportInstance.h"
#include "Viewport/Camera.h"

Rendering::MatrixUniformBuffer::MatrixUniformBuffer(ViewportInstance* parentViewport)
{
	CreateUniformBuffer(parentViewport);
}

Rendering::MatrixUniformBuffer::~MatrixUniformBuffer()
{
	DestroyUniformBuffer();
}

void Rendering::MatrixUniformBuffer::UpdateUniformBuffers(ViewportInstance* parentViewport, const size_t& ImageIndex)
{
	MatrixUniformBufferObject ubo{};
	ubo.viewMatrix = parentViewport->GetCamera()->GetViewMatrix();
	ubo.worldProjection = parentViewport->GetProjection();
	ubo.cameraLocation = glm::vec4(parentViewport->GetCamera()->GetPosition(), 0);
	ubo.time = (float)glfwGetTime();

	void* data;
	vkMapMemory(G_LOGICAL_DEVICE, uniformBuffersMemory[ImageIndex], 0, sizeof(ubo), 0, &data);
	memcpy(data, &ubo, sizeof(ubo));
	vkUnmapMemory(G_LOGICAL_DEVICE, uniformBuffersMemory[ImageIndex]);
}

void Rendering::MatrixUniformBuffer::CreateUniformBuffer(ViewportInstance* parentViewport)
{
	uniformBufferSize = sizeof(MatrixUniformBufferObject);

	uniformBuffers.resize(G_SWAP_CHAIN_IMAGE_COUNT);
	uniformBuffersMemory.resize(G_SWAP_CHAIN_IMAGE_COUNT);

	for (size_t i = 0; i < G_SWAP_CHAIN_IMAGE_COUNT; i++) {
		CreateBuffer(uniformBufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, uniformBuffers[i], uniformBuffersMemory[i]);
	}
}

void Rendering::MatrixUniformBuffer::DestroyUniformBuffer()
{
	for (size_t i = 0; i < uniformBuffers.size(); i++) {
		vkDestroyBuffer(G_LOGICAL_DEVICE, uniformBuffers[i], G_ALLOCATION_CALLBACK);
		vkFreeMemory(G_LOGICAL_DEVICE, uniformBuffersMemory[i], G_ALLOCATION_CALLBACK);
	}
}

