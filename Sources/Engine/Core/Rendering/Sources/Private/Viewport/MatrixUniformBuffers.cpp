#include "Viewport/MatrixUniformBuffers.h"
#include "Utils.h"
#include "Viewport/ViewportInstance.h"

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
	static auto startTime = std::chrono::high_resolution_clock::now();

	auto currentTime = std::chrono::high_resolution_clock::now();
	float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

	float rotVelocity = 10.f;

	float dist = (float)(sin(time / 2) + 1.5f) / 8 + 1.8f;

	MatrixUniformBufferObject ubo{};
	ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(rotVelocity), glm::vec3(0.0f, 0.0f, 1.0f));
	ubo.view = glm::lookAt(glm::vec3(dist, dist, dist), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	ubo.proj = glm::perspective(glm::radians(45.0f), parentViewport->GetViewportWidth() / (float)parentViewport->GetViewportHeight(), 0.1f, 10.0f);
	ubo.proj[1][1] *= -1;
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

