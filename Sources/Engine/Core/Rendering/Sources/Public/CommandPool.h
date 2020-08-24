#pragma once

#include "Constants.h"

namespace Rendering::CommandPool {
	struct CommandPoolItem {
		CommandPoolItem(std::thread::id threadId)
			: poolThreadId(threadId) {
			VkCommandPoolCreateInfo poolInfo{};
			poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
			poolInfo.queueFamilyIndex = G_QUEUE_FAMILY_INDICES.graphicsFamily.value();
			poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT; // Optional
			VK_ENSURE(vkCreateCommandPool(G_LOGICAL_DEVICE, &poolInfo, G_ALLOCATION_CALLBACK, &commandPool), "Failed to create command pool");
		}

		inline void Cleanup() {
			vkDestroyCommandPool(G_LOGICAL_DEVICE, commandPool, G_ALLOCATION_CALLBACK);
		}

		VkCommandPool commandPool = VK_NULL_HANDLE;

		inline operator bool() const {
			return std::this_thread::get_id() == poolThreadId;
		}

	private:

		std::thread::id poolThreadId;
	};

	inline CommandPoolItem* CommandPools;

	inline void CreateCommandPools() {
		CommandPools = (CommandPoolItem*)malloc(JobSystem::G_CPU_THREAD_COUNT * sizeof(CommandPoolItem));
		for (uint32_t i = 0; i < JobSystem::G_CPU_THREAD_COUNT; ++i) {
			new (CommandPools + i) CommandPoolItem(JobSystem::Workers::workers[i].GetWorkerThreadID());
		}
	}

	inline void CleanupCommandPools() {
		for (uint32_t i = 0; i < JobSystem::G_CPU_THREAD_COUNT; ++i) {
			CommandPools[i].Cleanup();
		}
		free(CommandPools);
	}
}

namespace Rendering {
	inline VkCommandPool& GetCommandPool() {
		for (uint32_t i = 0; i < JobSystem::G_CPU_THREAD_COUNT; ++i) {
			if (CommandPool::CommandPools[i]) return CommandPool::CommandPools[i].commandPool;
		}
		LOG_ASSERT("Failed to find command pool on current thread");
	}
}