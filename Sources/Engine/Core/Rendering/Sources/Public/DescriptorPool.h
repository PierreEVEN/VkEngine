#pragma once

#include "Constants.h"

namespace Rendering
{
	class DescriptorPoolDynamicHandle
	{
	public:
		static void FindOrCreatePool(VkDescriptorSetAllocateInfo& allocInfos)
		{
			if (allocInfos.descriptorSetCount > G_MAX_SET_PER_POOL)
			{
				LOG_ASSERT(String("Cannot allocate mor than ") + ToString(G_MAX_SET_PER_POOL) + " descriptors per pool.");
			}
			for (auto& pool : usedPools)
			{
				if (pool->HasSpaceFor(allocInfos.descriptorSetCount))
				{
					pool->BindAllocInfos(allocInfos);
					return;
				}
			}
			usedPools.push_back(new DescriptorPoolDynamicHandle(allocInfos));
		}

		static void ClearPools()
		{
			for (const auto& item : usedPools)
			{
				delete item;
			}
			usedPools.clear();
		}

	private:

		DescriptorPoolDynamicHandle(VkDescriptorSetAllocateInfo& allocInfos)
		{
			std::array<VkDescriptorPoolSize, 11> poolSizes;
			poolSizes[0] = { VK_DESCRIPTOR_TYPE_SAMPLER, G_POOL_DESCRIPTOR_COUNT_PER_TYPE };
			poolSizes[1] = { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, G_POOL_DESCRIPTOR_COUNT_PER_TYPE };
			poolSizes[2] = { VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, G_POOL_DESCRIPTOR_COUNT_PER_TYPE };
			poolSizes[3] = { VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, G_POOL_DESCRIPTOR_COUNT_PER_TYPE };
			poolSizes[4] = { VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, G_POOL_DESCRIPTOR_COUNT_PER_TYPE };
			poolSizes[5] = { VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, G_POOL_DESCRIPTOR_COUNT_PER_TYPE };
			poolSizes[6] = { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, G_POOL_DESCRIPTOR_COUNT_PER_TYPE };
			poolSizes[7] = { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, G_POOL_DESCRIPTOR_COUNT_PER_TYPE };
			poolSizes[8] = { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, G_POOL_DESCRIPTOR_COUNT_PER_TYPE };
			poolSizes[9] = { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, G_POOL_DESCRIPTOR_COUNT_PER_TYPE };
			poolSizes[10] = { VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, G_POOL_DESCRIPTOR_COUNT_PER_TYPE };
			spaceLeft = G_MAX_SET_PER_POOL;

			VkDescriptorPoolCreateInfo poolInfo{};
			poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
			poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
			poolInfo.pPoolSizes = poolSizes.data();
			poolInfo.maxSets = G_MAX_SET_PER_POOL;

			VK_ENSURE(vkCreateDescriptorPool(G_LOGICAL_DEVICE, &poolInfo, G_ALLOCATION_CALLBACK, &pool), "Failed to create descriptor pool");

			BindAllocInfos(allocInfos);
		}

		~DescriptorPoolDynamicHandle()
		{
			vkDestroyDescriptorPool(G_LOGICAL_DEVICE, pool, G_ALLOCATION_CALLBACK);
		}

		inline bool HasSpaceFor(const uint32_t& neededSpace) const { return spaceLeft >= neededSpace; }

		inline void BindAllocInfos(VkDescriptorSetAllocateInfo& allocInfos)
		{
			spaceLeft -= allocInfos.descriptorSetCount;
			allocInfos.descriptorPool = pool;
		}

		inline static std::vector<DescriptorPoolDynamicHandle*> usedPools;
		VkDescriptorPool pool = VK_NULL_HANDLE;
		uint32_t spaceLeft = 0;
	};
	inline void ReserveDescriptorPoolMemory(VkDescriptorSetAllocateInfo& allocInfos) { DescriptorPoolDynamicHandle::FindOrCreatePool(allocInfos); }
}