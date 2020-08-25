#pragma once

#include "Assets/Asset.h"
#include "AssetFactory.h"
#include <shaderc/shaderc.hpp>
#include "Assets\ShaderModule.refl.h" // automatically generated reflection header

namespace Rendering {


	DECLARE_DELEGATE_MULTICAST(OnRecompiledShaderModuleEvent);

	REFLECT()
		class ShaderModule : public Asset {
		REFLECT_BODY()
		public:
			friend TAssetFactory<ShaderModule>;
			inline ~ShaderModule();

			virtual Texture2D* GetAssetIcon() const;
			inline String GetShaderCode() const { return shaderCode.data(); }
			inline VkShaderModule& GetShaderModule() { return shaderModule; }

			inline bool operator==(const ShaderModule& other) const { return shaderModule == other.shaderModule; }

			void UpdateShaderCode(const std::vector<char>& newShaderCode);

			OnRecompiledShaderModuleEvent OnRecompiledShaderModule;

		protected:

			static void CompileShader(VkShaderModule& outModule, const std::vector<char> shaderData, const shaderc_shader_kind& shaderStage, const String& fileName, bool bOptimize = true);

		private:

			std::vector<char> shaderCode;
			shaderc_shader_kind moduleShaderStage;
			VkShaderModule shaderModule = VK_NULL_HANDLE;
			ShaderModule(const std::vector<char>& shaderTextData, const shaderc_shader_kind& shaderStage, const String& assetName);
			static ShaderModule* ImportFromPath(const String& path);
			inline static Texture2D* shaderIcon = nullptr;


	};


	template<>
	struct TAssetFactory<ShaderModule> {
		inline static std::vector<String> GetDesiredImportFormats() { return { "spv" }; }
		inline static ShaderModule* ImportFromPath(const String& path) { return ShaderModule::ImportFromPath(path); }
		inline static ShaderModule* MakeTransient(ShaderModule* inShad) { inShad->MakeConstant(); return inShad; }
	};
}
