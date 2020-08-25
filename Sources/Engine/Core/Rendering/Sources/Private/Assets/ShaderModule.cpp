#include "Assets/ShaderModule.h"
#include "Assets/Texture2D.h"


Rendering::ShaderModule::ShaderModule(const std::vector<char>& shaderTextData, const shaderc_shader_kind& shaderStage, const String& assetName)
	: Asset(assetName), shaderCode(shaderTextData), moduleShaderStage(shaderStage)
{
	CompileShader(shaderModule, shaderTextData, shaderStage, assetName);
}

Rendering::ShaderModule* Rendering::ShaderModule::ImportFromPath(const String& path)
{
	return new ShaderModule(ReadFile(path), String::GetFileExtension(path) == "vert" ? shaderc_vertex_shader : shaderc_fragment_shader, String::GetFileShortName(path));
}



Rendering::ShaderModule::~ShaderModule() {
	vkDestroyShaderModule(G_LOGICAL_DEVICE, shaderModule, G_ALLOCATION_CALLBACK);
}

Rendering::Texture2D* Rendering::ShaderModule::GetAssetIcon() const
{
	if (!shaderIcon) {
		shaderIcon = TAssetFactory<Texture2D>::ImportFromPath("Ressources/Textures/Icons/Assets/icon-shader.png");
	}
	return shaderIcon;
}


void Rendering::ShaderModule::UpdateShaderCode(const std::vector<char>& newShaderCode)
{
	shaderCode = newShaderCode;


	vkDestroyShaderModule(G_LOGICAL_DEVICE, shaderModule, G_ALLOCATION_CALLBACK);

	CompileShader(shaderModule, newShaderCode, moduleShaderStage, GetName());
	OnRecompiledShaderModule.Execute();

}

void Rendering::ShaderModule::CompileShader(VkShaderModule& outModule, const std::vector<char> shaderData, const shaderc_shader_kind& shaderStage, const String& fileName, bool bOptimize)
{
	shaderc::Compiler Compiler;
	shaderc::CompileOptions Options;

	Options.AddMacroDefinition(shaderStage == shaderc_vertex_shader ? "VERTEX_SHADER" : "FRAGMENT_SHADER");

	shaderc::PreprocessedSourceCompilationResult PreprocResult = Compiler.PreprocessGlsl(shaderData.data(), shaderStage, fileName.GetData(), Options);

	if (PreprocResult.GetCompilationStatus() != shaderc_compilation_status_success)
	{
		LOG_ASSERT("Failed to preprocess shader " + fileName + " : " + PreprocResult.GetErrorMessage().c_str());
	}
	std::vector<char> PreprocessedShader = { PreprocResult.cbegin(), PreprocResult.cend() };

	Options.SetTargetEnvironment(shaderc_target_env_vulkan, 0);

	/** If in debug mode, don't optimize */
#ifdef _DEBUG
	Options.SetOptimizationLevel(bOptimize ?
		shaderc_optimization_level_size : shaderc_optimization_level_zero);
#else
	Options.SetOptimizationLevel(bOptimize ?
		shaderc_optimization_level_performance : shaderc_optimization_level_zero);
#endif

	shaderc::SpvCompilationResult Result = Compiler.CompileGlslToSpv((char*)PreprocessedShader.data(), PreprocessedShader.size(), shaderStage, fileName.GetData(), "main", Options);

	if (Result.GetCompilationStatus() != shaderc_compilation_status_success)
	{
		LOG_ASSERT("Failed to compile shader " + fileName + " : " + Result.GetErrorMessage().c_str());
	}
	std::vector<uint32_t> bytecode { Result.cbegin(), Result.cend() };


	VkShaderModuleCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize = bytecode.size() * sizeof(uint32_t);
	createInfo.pCode = reinterpret_cast<const uint32_t*>(bytecode.data());

	VK_ENSURE(vkCreateShaderModule(G_LOGICAL_DEVICE, &createInfo, G_ALLOCATION_CALLBACK, &outModule), "Failed to create shader module");
}

