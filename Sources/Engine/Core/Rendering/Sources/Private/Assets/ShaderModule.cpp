#include "Assets/ShaderModule.h"
#include "Assets/Texture2D.h"

Rendering::ShaderModule::ShaderModule(const std::vector<char>& shaderCode, const String& assetName)
	: Asset(assetName)
{
	VkShaderModuleCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize = shaderCode.size();
	createInfo.pCode = reinterpret_cast<const uint32_t*>(shaderCode.data());
	VK_ENSURE(vkCreateShaderModule(G_LOGICAL_DEVICE, &createInfo, G_ALLOCATION_CALLBACK, &shaderModule), "Failed to create vertex shader module");
}

Rendering::ShaderModule* Rendering::ShaderModule::ImportFromPath(const String& path)
{
	std::ifstream file(path.GetData(), std::ios::ate | std::ios::binary);

	if (!file.is_open()) {
		LOG_ASSERT(String("Failed to open shader file ") + path);
	}
	size_t fileSize = (size_t)file.tellg();
	std::vector<char> buffer(fileSize);
	file.seekg(0);
	file.read(buffer.data(), fileSize);
	file.close();
	return new ShaderModule(buffer, String::GetFileShortName(path));
}

Rendering::Texture2D* Rendering::ShaderModule::GetAssetIcon() const
{
	if (!shaderIcon) {
		shaderIcon = TAssetFactory<Texture2D>::ImportFromPath("Ressources/Textures/Icons/Assets/icon-shader.png");
	}
	return shaderIcon;
}

