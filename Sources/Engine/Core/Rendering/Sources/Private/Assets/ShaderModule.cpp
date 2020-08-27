#include "Assets/ShaderModule.h"
#include "Assets/Texture2D.h"
#include <spirv_cross/spirv_glsl.hpp>


Rendering::ShaderModule::ShaderModule(const std::vector<char>& shaderTextData, const String& assetName, const EShaderType& shaderType)
	: Asset(assetName), shaderCode(shaderTextData)
{
	CompileShader(shaderModule, shaderType, shaderTextData, assetName);
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
	VkShaderModule newModule;
	if (CompileShader(newModule, GetShaderKind(), newShaderCode, GetName())) {
		shaderCode = newShaderCode;
		outDatedShaderModules.push_back(shaderModule);
		shaderModule = newModule;
		OnRecompiledShaderModule.Execute();
	}

}

void Rendering::ShaderModule::FlushOutdatedShaderModules()
{
	for (auto& module : outDatedShaderModules)
		vkDestroyShaderModule(G_LOGICAL_DEVICE, module, G_ALLOCATION_CALLBACK);
	outDatedShaderModules.clear();
}



void GetReflectionData(const std::vector<uint32_t>& InSpv)
{

	spirv_cross::Compiler Compiler(InSpv);

// 	spirv_cross::ShaderResources Resources = Compiler.get_shader_resources();
// 
// 
// 	LOG("uniform_buffers : " + ToString(Resources.uniform_buffers.size()));
// 	LOG("stage_inputs : " + ToString(Resources.stage_inputs.size()));
// 	LOG("stage_outputs : " + ToString(Resources.stage_outputs.size()));
// 	LOG("subpass_inputs : " + ToString(Resources.subpass_inputs.size()));
// 	LOG("storage_images : " + ToString(Resources.storage_images.size()));
// 	LOG("sampled_images : " + ToString(Resources.sampled_images.size()));
// 	LOG("atomic_counters : " + ToString(Resources.atomic_counters.size()));
// 	LOG("acceleration_structures : " + ToString(Resources.acceleration_structures.size()));




// 	for (auto& UniformBuffer : Resources.uniform_buffers)
// 	{
// 		const auto& Type = Compiler.get_type(UniformBuffer.base_type_id);
// 
// 		unsigned Set = Compiler.get_decoration(UniformBuffer.id, spv::DecorationDescriptorSet);
// 		unsigned Binding = Compiler.get_decoration(UniformBuffer.id, spv::DecorationBinding);
// 		std::string Name = Compiler.get_name(UniformBuffer.id);
// 		if (Name.empty())
// 			Name = Compiler.get_name(UniformBuffer.base_type_id);
// 
// 		uint64_t Size = Compiler.get_declared_struct_size(Type);

// 		std::vector<SShaderParameterMember> Members; /*=
// 			ParseStruct(Compiler.get(), Type);*/
// 
// 		Output.ParameterMap.AddParameter(Name.c_str(),
// 			{
// 				Name.c_str(),
// 				EShaderParameterType::UniformBuffer,
// 				Set,
// 				Binding,
// 				Size,
// 				1,
// 				Members
// 			});
/*	}*/

}



bool Rendering::ShaderModule::CompileShader(VkShaderModule& outModule, const EShaderType& shaderType, const std::vector<char> shaderData, const String& fileName, bool bOptimize)
{
	shaderc::Compiler Compiler;
	shaderc::CompileOptions Options;

	const shaderc_shader_kind& shaderStage = (shaderType == EShaderType::Vertex ? shaderc_vertex_shader : shaderc_fragment_shader);

	Options.AddMacroDefinition(shaderStage == shaderc_vertex_shader ? "VERTEX_SHADER" : "FRAGMENT_SHADER");

	shaderc::PreprocessedSourceCompilationResult PreprocResult = Compiler.PreprocessGlsl(shaderData.data(), shaderStage, fileName.GetData(), Options);

	if (PreprocResult.GetCompilationStatus() != shaderc_compilation_status_success)
	{
		String errorString = "Failed to preprocess shader " + fileName + " : " + PreprocResult.GetErrorMessage().c_str();
		OnShaderCompillationGetError.Execute(errorString);
		LOG_ERROR(errorString);
		return false;
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
		String errorString("Failed to compile shader " + fileName + " : " + Result.GetErrorMessage().c_str());
		OnShaderCompillationGetError.Execute(errorString);
		LOG_ERROR(errorString);
		return false;
	}
	std::vector<uint32_t> bytecode { Result.cbegin(), Result.cend() };


	GetReflectionData(bytecode);

	VkShaderModuleCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize = bytecode.size() * sizeof(uint32_t);
	createInfo.pCode = reinterpret_cast<const uint32_t*>(bytecode.data());
	VkResult res = vkCreateShaderModule(G_LOGICAL_DEVICE, &createInfo, G_ALLOCATION_CALLBACK, &outModule);
	if (!res == VK_SUCCESS) {
		String errorString("Failed to create shader module " + GetName() + ". Error code : " + (uint32_t)res);
		OnShaderCompillationGetError.Execute(errorString);
		LOG_ERROR(errorString);
		return false;
	}
	return true;
}