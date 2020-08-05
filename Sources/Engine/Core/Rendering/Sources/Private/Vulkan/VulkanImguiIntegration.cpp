#include "Vulkan/VulkanImguiIntegration.h"

#include <imgui/imgui.h>
#include <imgui/examples/imgui_impl_glfw.h>
#include "Vulkan/VulkanInstance.h"
#include "Rendering.h"
#include "Vulkan/VulkanPhysicalDevice.h"
#include "Vulkan/VulkanLogicalDevice.h"
#include "Vulkan/VulkanUtils.h"
#include "IO/Log.h"
#include <array>
#include "Vulkan/VulkanSwapChain.h"
#include "Vulkan/VulkanRenderPass.h"
#include "Vulkan/VulkanCommandBuffer.h"
#include "Vulkan/VulkanAntialiasing.h"
#include "Vulkan/VulkanSurface.h"
#include "imgui/imgui_internal.h"
#include "Vulkan/VulkanCommandPool.h"


VkDescriptorPool imGuiDescriptorPool;

VkBuffer ImGuiVertexBuffer = VK_NULL_HANDLE;
VkDeviceMemory ImGuiVertexBufferMemory;
VkDeviceSize ImGuiVertexBufferSize;

VkBuffer ImGuiIndexBuffer = VK_NULL_HANDLE;
VkDeviceMemory ImGuiIndexBufferMemory;
VkDeviceSize ImGuiIndexBufferSize;


void Rendering::Vulkan::ImGuiIntegration::CreateImGuiDescriptorPool()
{
	LOG("Create ImGui Descriptor pool");
	std::array<VkDescriptorPoolSize, 11> poolSizes;
	poolSizes[0] = { VK_DESCRIPTOR_TYPE_SAMPLER, 1000 };
	poolSizes[1] = { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 };
	poolSizes[2] = { VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 };
	poolSizes[3] = { VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 };
	poolSizes[4] = { VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 };
	poolSizes[5] = { VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 };
	poolSizes[6] = { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 };
	poolSizes[7] = { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 };
	poolSizes[8] = { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 };
	poolSizes[9] = { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 };
	poolSizes[10] = { VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 };

	VkDescriptorPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
	poolInfo.pPoolSizes = poolSizes.data();
	poolInfo.maxSets = static_cast<uint32_t>(Rendering::Vulkan::SwapChain::GetImageViews().size());

	if (vkCreateDescriptorPool(Rendering::Vulkan::LogDevice::GetLogicalDevice(), &poolInfo, nullptr, &imGuiDescriptorPool) != VK_SUCCESS) {
		LOG_ASSERT("Failed to create imgui descriptor pool");
	}
}

void Rendering::Vulkan::ImGuiIntegration::CreateContext()
{
	LOG("Create ImGui context");
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

}

void CheckImGUiVkResult(VkResult err)
{
	if (err != VK_SUCCESS) { LOG_ASSERT(String("ImGui initialization error : ") + String::ToString((int32_t)err)); }
}

void Rendering::Vulkan::ImGuiIntegration::Init()
{
	CreateContext();

	ImGuiIntegration::CreateImGuiDescriptorPool();

	LOG("Initialize ImGui");
	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForVulkan(GetPrimaryWindow(), true);
	ImGui_ImplVulkan_InitInfo init_info = {};
	init_info.Instance = Instance::GetInstance();
	init_info.PhysicalDevice = PhysDevice::GetPhysicalDevice();
	init_info.Device = LogDevice::GetLogicalDevice();
	init_info.QueueFamily =	Utils::FindDeviceQueueFamilies(PhysDevice::GetPhysicalDevice()).graphicsFamily.value();
	init_info.Queue = LogDevice::GetGraphicQueues();
	init_info.PipelineCache = VK_NULL_HANDLE;
	init_info.DescriptorPool = imGuiDescriptorPool;
	init_info.MSAASamples = Antialiasing::GetMsaaSampleCount();
	init_info.Allocator = nullptr;
	init_info.MinImageCount = (uint32_t)SwapChain::GetImageViews().size();
	init_info.ImageCount = (uint32_t)SwapChain::GetImageViews().size();
	init_info.CheckVkResultFn = CheckImGUiVkResult;
	ImGui_ImplVulkan_Init(&init_info, RenderPass::GetRenderPass());

	// Upload Fonts
	{
		VkCommandBuffer command_buffer = Utils::BeginSingleTimeCommands();
		ImGui_ImplVulkan_CreateFontsTexture(command_buffer);
		Utils::EndSingleTimeCommands(command_buffer);
	}

	//if (!GImGui->Font) LOG_ASSERT("Failed to upload imgui font");
}

void Rendering::Vulkan::ImGuiIntegration::Rebuild()
{
// 	ImGui_ImplVulkan_SetMinImageCount(SwapChain::GetImageViews().size());
// 	ImGui_ImplVulkanH_CreateOrResizeWindow(Instance::GetInstance(), PhysDevice::GetPhysicalDevice(), LogDevice::GetLogicalDevice(), &g_MainWindow, Utils::FindQueueFamilies(PhysDevice::GetPhysicalDevice()).graphicsFamily.value(),
// 		nullptr, SwapChain::GetSwapchainExtend().width, SwapChain::GetSwapchainExtend().height, SwapChain::GetImageViews().size());
// 	g_MainWindow.FrameIndex = 0;
}

void Rendering::Vulkan::ImGuiIntegration::Cleanup()
{
	LOG("Cleanup ImGui");
	ImGui_ImplVulkan_Shutdown();
	ImGui_ImplGlfw_Shutdown();


	vkDestroyBuffer(LogDevice::GetLogicalDevice(), ImGuiVertexBuffer, nullptr);
	vkFreeMemory(LogDevice::GetLogicalDevice(), ImGuiVertexBufferMemory, nullptr);
	vkDestroyBuffer(LogDevice::GetLogicalDevice(), ImGuiIndexBuffer, nullptr);
	vkFreeMemory(LogDevice::GetLogicalDevice(), ImGuiIndexBufferMemory, nullptr);

	vkDestroyDescriptorPool(LogDevice::GetLogicalDevice(), imGuiDescriptorPool, nullptr);
}



static VkDeviceSize g_BufferMemoryAlignment = 256;

void Rendering::Vulkan::ImGuiIntegration::CreateOrResizeBuffer(VkBuffer& buffer, VkDeviceMemory& buffer_memory, VkDeviceSize& p_buffer_size, size_t new_size, VkBufferUsageFlagBits usage)
{
	if (buffer != VK_NULL_HANDLE)
		vkDestroyBuffer(LogDevice::GetLogicalDevice(), buffer, nullptr);
	if (buffer_memory != VK_NULL_HANDLE)
		vkFreeMemory(LogDevice::GetLogicalDevice(), buffer_memory, nullptr);

	VkDeviceSize vertex_buffer_size_aligned = ((new_size - 1) / g_BufferMemoryAlignment + 1) * g_BufferMemoryAlignment;
	VkBufferCreateInfo buffer_info = {};
	buffer_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	buffer_info.size = vertex_buffer_size_aligned;
	buffer_info.usage = usage;
	buffer_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	vkCreateBuffer(LogDevice::GetLogicalDevice(), &buffer_info, nullptr, &buffer);

	VkMemoryRequirements req;
	vkGetBufferMemoryRequirements(LogDevice::GetLogicalDevice(), buffer, &req);
	g_BufferMemoryAlignment = (g_BufferMemoryAlignment > req.alignment) ? g_BufferMemoryAlignment : req.alignment;
	VkMemoryAllocateInfo alloc_info = {};
	alloc_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	alloc_info.allocationSize = req.size;
	alloc_info.memoryTypeIndex = Utils::FindMemoryType(req.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
	 vkAllocateMemory(LogDevice::GetLogicalDevice(), &alloc_info, nullptr, &buffer_memory);

	vkBindBufferMemory(LogDevice::GetLogicalDevice(), buffer, buffer_memory, 0);
	p_buffer_size = new_size;
}

void Rendering::Vulkan::ImGuiIntegration::Render()
{
// 	// Rendering
// 	ImGui::Render();
// 	ImDrawData* draw_data = ImGui::GetDrawData();
// 
// 
// 	//ImGui_ImplVulkan_RenderDrawData(draw_data, bfr);
// 
// 
// 
// 	// Avoid rendering when minimized, scale coordinates for retina displays (screen coordinates != framebuffer coordinates)
// 	int fb_width = (int)(draw_data->DisplaySize.x * draw_data->FramebufferScale.x);
// 	int fb_height = (int)(draw_data->DisplaySize.y * draw_data->FramebufferScale.y);
// 	if (fb_width <= 0 || fb_height <= 0)
// 		return;
// 
// 	if (draw_data->TotalVtxCount > 0)
// 	{
// 		// Create or resize the vertex/index buffers
// 		size_t vertex_size = draw_data->TotalVtxCount * sizeof(ImDrawVert);
// 		size_t index_size = draw_data->TotalIdxCount * sizeof(ImDrawIdx);
// 		if (ImGuiVertexBuffer == VK_NULL_HANDLE || ImGuiVertexBufferSize < vertex_size)
// 			CreateOrResizeBuffer(ImGuiVertexBuffer, ImGuiVertexBufferMemory, ImGuiVertexBufferSize, vertex_size, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
// 		if (ImGuiIndexBuffer == VK_NULL_HANDLE || ImGuiIndexBufferSize < index_size)
// 			CreateOrResizeBuffer(ImGuiIndexBuffer, ImGuiIndexBufferMemory, ImGuiIndexBufferSize, index_size, VK_BUFFER_USAGE_INDEX_BUFFER_BIT);
// 
// 		// Upload vertex/index data into a single contiguous GPU buffer
// 		ImDrawVert* vtx_dst = NULL;
// 		ImDrawIdx* idx_dst = NULL;
// 		vkMapMemory(LogDevice::GetLogicalDevice(), ImGuiVertexBufferMemory, 0, vertex_size, 0, (void**)(&vtx_dst));
// 		vkMapMemory(LogDevice::GetLogicalDevice(), ImGuiIndexBufferMemory, 0, index_size, 0, (void**)(&idx_dst));
// 
// 		for (int n = 0; n < draw_data->CmdListsCount; n++)
// 		{
// 			const ImDrawList* cmd_list = draw_data->CmdLists[n];
// 			memcpy(vtx_dst, cmd_list->VtxBuffer.Data, cmd_list->VtxBuffer.Size * sizeof(ImDrawVert));
// 			memcpy(idx_dst, cmd_list->IdxBuffer.Data, cmd_list->IdxBuffer.Size * sizeof(ImDrawIdx));
// 			vtx_dst += cmd_list->VtxBuffer.Size;
// 			idx_dst += cmd_list->IdxBuffer.Size;
// 		}
// 		VkMappedMemoryRange range[2] = {};
// 		range[0].sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
// 		range[0].memory = ImGuiVertexBufferMemory;
// 		range[0].size = VK_WHOLE_SIZE;
// 		range[1].sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
// 		range[1].memory = ImGuiIndexBufferMemory;
// 		range[1].size = VK_WHOLE_SIZE;
// 		vkFlushMappedMemoryRanges(LogDevice::GetLogicalDevice(), 2, range);
// 		vkUnmapMemory(LogDevice::GetLogicalDevice(), ImGuiVertexBufferMemory);
// 		vkUnmapMemory(LogDevice::GetLogicalDevice(), ImGuiIndexBufferMemory);
// 	}
// 
// 
// 
// 
// 
// 	// Setup desired Vulkan state
// 	ImGui_ImplVulkan_SetupRenderState(draw_data, command_buffer, rb, fb_width, fb_height);
// 
// 	// Will project scissor/clipping rectangles into framebuffer space
// 	ImVec2 clip_off = draw_data->DisplayPos;         // (0,0) unless using multi-viewports
// 	ImVec2 clip_scale = draw_data->FramebufferScale; // (1,1) unless using retina display which are often (2,2)
// 
// 	// Render command lists
// 	// (Because we merged all buffers into a single one, we maintain our own offset into them)
// 	int global_vtx_offset = 0;
// 	int global_idx_offset = 0;
// 	for (int n = 0; n < draw_data->CmdListsCount; n++)
// 	{
// 		const ImDrawList* cmd_list = draw_data->CmdLists[n];
// 		for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++)
// 		{
// 			const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[cmd_i];
// 			if (pcmd->UserCallback != NULL)
// 			{
// 				// User callback, registered via ImDrawList::AddCallback()
// 				// (ImDrawCallback_ResetRenderState is a special callback value used by the user to request the renderer to reset render state.)
// 				if (pcmd->UserCallback == ImDrawCallback_ResetRenderState)
// 					ImGui_ImplVulkan_SetupRenderState(draw_data, command_buffer, rb, fb_width, fb_height);
// 				else
// 					pcmd->UserCallback(cmd_list, pcmd);
// 			}
// 			else
// 			{
// 				// Project scissor/clipping rectangles into framebuffer space
// 				ImVec4 clip_rect;
// 				clip_rect.x = (pcmd->ClipRect.x - clip_off.x) * clip_scale.x;
// 				clip_rect.y = (pcmd->ClipRect.y - clip_off.y) * clip_scale.y;
// 				clip_rect.z = (pcmd->ClipRect.z - clip_off.x) * clip_scale.x;
// 				clip_rect.w = (pcmd->ClipRect.w - clip_off.y) * clip_scale.y;
// 
// 				if (clip_rect.x < fb_width && clip_rect.y < fb_height && clip_rect.z >= 0.0f && clip_rect.w >= 0.0f)
// 				{
// 					// Negative offsets are illegal for vkCmdSetScissor
// 					if (clip_rect.x < 0.0f)
// 						clip_rect.x = 0.0f;
// 					if (clip_rect.y < 0.0f)
// 						clip_rect.y = 0.0f;
// 
// 					// Apply scissor/clipping rectangle
// 					VkRect2D scissor;
// 					scissor.offset.x = (int32_t)(clip_rect.x);
// 					scissor.offset.y = (int32_t)(clip_rect.y);
// 					scissor.extent.width = (uint32_t)(clip_rect.z - clip_rect.x);
// 					scissor.extent.height = (uint32_t)(clip_rect.w - clip_rect.y);
// 					vkCmdSetScissor(command_buffer, 0, 1, &scissor);
// 
// 					// Draw
// 					vkCmdDrawIndexed(command_buffer, pcmd->ElemCount, 1, pcmd->IdxOffset + global_idx_offset, pcmd->VtxOffset + global_vtx_offset, 0);
// 				}
// 			}
// 		}
// 		global_idx_offset += cmd_list->IdxBuffer.Size;
// 		global_vtx_offset += cmd_list->VtxBuffer.Size;
// 	}
}