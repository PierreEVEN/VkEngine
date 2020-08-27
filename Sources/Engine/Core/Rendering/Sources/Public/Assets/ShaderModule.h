#pragma once

#include "Assets/Asset.h"
#include "AssetFactory.h"
#include <shaderc/shaderc.hpp>
#include "Assets\ShaderModule.refl.h" // automatically generated reflection header

namespace Rendering {


	DECLARE_DELEGATE_MULTICAST(OnRecompiledShaderModuleEvent);
	DECLARE_DELEGATE_MULTICAST(OnShaderCompillationGetErrorsEvent, const String&);

	enum class EShaderType {
		Vertex = 0,
		Fragment = 1
	};


	REFLECT()
	class ShaderModule : public Asset {
		REFLECT_BODY()
	public:
		friend VertexShaderModule;
		friend FragmentShaderModule;
		inline virtual ~ShaderModule();

		virtual Texture2D* GetAssetIcon() const;
		inline String GetShaderCode() const { return shaderCode.data(); }
		inline VkShaderModule& GetShaderModule() { return shaderModule; }

		inline bool operator==(const ShaderModule& other) const { return shaderModule == other.shaderModule; }

		void UpdateShaderCode(const std::vector<char>& newShaderCode);

		OnRecompiledShaderModuleEvent OnRecompiledShaderModule;

		static void FlushOutdatedShaderModules();

		inline virtual const EShaderType GetShaderKind() const = 0;

		OnShaderCompillationGetErrorsEvent OnShaderCompillationGetError;

	protected:

		bool CompileShader(VkShaderModule& outModule, const EShaderType& shaderType, const std::vector<char> shaderData, const String& fileName, bool bOptimize = true);

	private:
		ShaderModule(const std::vector<char>& shaderTextData, const String& assetName, const EShaderType& shaderType);

		ShaderModule* defaultVertexShader = nullptr;
		ShaderModule* defaultFragmentShader = nullptr;

		std::vector<char> shaderCode;
		EShaderType moduleShaderStage;
		VkShaderModule shaderModule = VK_NULL_HANDLE;
		inline static Texture2D* shaderIcon = nullptr;
		inline static std::vector<VkShaderModule> outDatedShaderModules;

	};

	REFLECT()
		class VertexShaderModule : public ShaderModule {
		REFLECT_BODY()
		public:
			friend TAssetFactory<VertexShaderModule>;

			inline virtual const EShaderType GetShaderKind() const { return EShaderType::Vertex; }
		private:
			VertexShaderModule(const std::vector<char>& shaderTextData, const String& assetName) : ShaderModule(shaderTextData, assetName, EShaderType::Vertex) {}
			static VertexShaderModule* ImportFromPath(const String& path) {
				return new VertexShaderModule(ReadFile(path), String::GetFileShortName(path));
			}
	};

	REFLECT()
		class FragmentShaderModule : public ShaderModule {
		REFLECT_BODY()
		public:
			friend TAssetFactory<FragmentShaderModule>;

			inline virtual const EShaderType GetShaderKind() const { return EShaderType::Fragment; }
		private:
			FragmentShaderModule(const std::vector<char>& shaderTextData, const String& assetName) : ShaderModule(shaderTextData, assetName, EShaderType::Fragment) {}
			static FragmentShaderModule* ImportFromPath(const String& path) {
				return new FragmentShaderModule(ReadFile(path), String::GetFileShortName(path));
			}
	};

	template<>
	struct TAssetFactory<VertexShaderModule> {
		inline static std::vector<String> GetDesiredImportFormats() { return { "vert" }; }
		inline static VertexShaderModule* ImportFromPath(const String& path) { return VertexShaderModule::ImportFromPath(path); }
		inline static VertexShaderModule* MakeTransient(VertexShaderModule* inShad) { inShad->MakeConstant(); return inShad; }
	};

	template<>
	struct TAssetFactory<FragmentShaderModule> {
		inline static std::vector<String> GetDesiredImportFormats() { return { "frag" }; }
		inline static FragmentShaderModule* ImportFromPath(const String& path) { return FragmentShaderModule::ImportFromPath(path); }
		inline static FragmentShaderModule* MakeTransient(FragmentShaderModule* inShad) { inShad->MakeConstant(); return inShad; }
	};
}
