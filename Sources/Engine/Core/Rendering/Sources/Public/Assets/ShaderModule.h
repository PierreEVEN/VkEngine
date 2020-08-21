#pragma once

#include "Assets/Asset.h"
#include "AssetFactory.h"
#include "Assets\ShaderModule.refl.h" // automatically generated reflection header

namespace Rendering {


	REFLECT()
		class ShaderModule : public Asset {
		REFLECT_BODY()
		public:

			friend TAssetFactory<ShaderModule>;

			inline ~ShaderModule() {
				vkDestroyShaderModule(G_LOGICAL_DEVICE, shaderModule, G_ALLOCATION_CALLBACK);
			}

			inline bool operator==(const ShaderModule& other) const {
				return shaderModule == other.shaderModule;
			}

			VkShaderModule shaderModule = VK_NULL_HANDLE;

			virtual Texture2D* GetAssetIcon() const;

		private:

			ShaderModule(const std::vector<char>& shaderFilePath, const String& assetName);

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
