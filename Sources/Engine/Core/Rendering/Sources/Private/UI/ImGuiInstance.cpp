#include "UI/ImGuiInstance.h"
#include "Scene/Scene.h"

#include "Rendering.h"
#include "Utils.h"

void ImGuiVkResultDelegate(VkResult err)
{
	if (err != VK_SUCCESS) { LOG_ASSERT(String("ImGui initialization error : ") + ToString((int32_t)err)); }
}

void Rendering::PreInitializeImGui()
{
	LOG("Initialize imgui ressources");
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

	ImGuiStyle& style = ImGui::GetStyle();
	style.WindowRounding = 0;
	style.ScrollbarRounding = 0;
	style.TabRounding = 0;
	style.WindowBorderSize = 1;
	style.PopupBorderSize = 1;
	style.WindowTitleAlign.x = 0.5f;
	style.FramePadding.x = 6.f;
	style.FramePadding.y = 6.f;
	style.WindowPadding.x = 4.f;
	style.WindowPadding.y = 4.f;
	style.GrabMinSize = 16.f;
	style.ScrollbarSize = 20.f;
	style.IndentSpacing = 30.f;

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	G_IMGUI_DEFAULT_FONT = io.Fonts->AddFontFromFileTTF(G_DEFAULT_FONT_PATH.GetValue().GetData(), 20.f);


	const size_t descPoolCount = 20;

	std::array<VkDescriptorPoolSize, 11> poolSizes;
	poolSizes[0] = { VK_DESCRIPTOR_TYPE_SAMPLER, descPoolCount };
	poolSizes[1] = { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, descPoolCount };
	poolSizes[2] = { VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, descPoolCount };
	poolSizes[3] = { VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, descPoolCount };
	poolSizes[4] = { VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, descPoolCount };
	poolSizes[5] = { VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, descPoolCount };
	poolSizes[6] = { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, descPoolCount };
	poolSizes[7] = { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, descPoolCount };
	poolSizes[8] = { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, descPoolCount };
	poolSizes[9] = { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, descPoolCount };
	poolSizes[10] = { VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, descPoolCount };

	VkDescriptorPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
	poolInfo.pPoolSizes = poolSizes.data();
	poolInfo.maxSets = static_cast<uint32_t>(G_SWAP_CHAIN_IMAGE_COUNT);

	VK_ENSURE(vkCreateDescriptorPool(G_LOGICAL_DEVICE, &poolInfo, G_ALLOCATION_CALLBACK, &G_IMGUI_DESCRIPTOR_POOL), "Failed to create imgui descriptor pool");

	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForVulkan(GetPrimaryWindow(), true);
	ImGui_ImplVulkan_InitInfo init_info = {};
	init_info.Instance = G_INSTANCE;
	init_info.PhysicalDevice = G_PHYSICAL_DEVICE;
	init_info.Device = G_LOGICAL_DEVICE;
	init_info.QueueFamily = G_QUEUE_FAMILY_INDICES.graphicsFamily.value();
	init_info.Queue = G_GRAPHIC_QUEUE;
	init_info.PipelineCache = VK_NULL_HANDLE;
	init_info.DescriptorPool = G_IMGUI_DESCRIPTOR_POOL;
	init_info.MSAASamples = (VkSampleCountFlagBits)G_MSAA_SAMPLE_COUNT.GetValue();
	init_info.Allocator = nullptr;
	init_info.MinImageCount = static_cast<uint32_t>(G_SWAP_CHAIN_IMAGE_COUNT);
	init_info.ImageCount = static_cast<uint32_t>(G_SWAP_CHAIN_IMAGE_COUNT);
	init_info.CheckVkResultFn = ImGuiVkResultDelegate;
	ImGui_ImplVulkan_Init(&init_info, G_RENDER_PASS);

	// Upload Fonts
	{
		VkCommandBuffer command_buffer = BeginSingleTimeCommands();
		ImGui_ImplVulkan_CreateFontsTexture(command_buffer);
		EndSingleTimeCommands(command_buffer);
	}
}

void Rendering::DestroyImGuiRessources()
{
	LOG("Cleanup ImGui ressources");
	ImGui_ImplVulkan_Shutdown();
	ImGui_ImplGlfw_Shutdown();

	vkDestroyDescriptorPool(G_LOGICAL_DEVICE, G_IMGUI_DESCRIPTOR_POOL, G_ALLOCATION_CALLBACK);
}
