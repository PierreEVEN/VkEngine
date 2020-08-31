#pragma once

#include "SubWindow.h"

namespace Rendering {
	class Material;
	class ShaderModule;
	class MaterialEditor;

	struct ShaderEditor {
	public:

		ShaderEditor(ShaderModule* sourceFile, String& inLogText, bool& bInIsCompiling, MaterialEditor* inParent);

		inline bool hasBeenModified() const { return bHasBeenModified; }

		virtual void DrawContent(const size_t& imageIndex);

	protected:


	private:

		inline void OnSuccessfullyCompiledShader();
		inline void OnShaderCompilationError(const String& error);
		ShaderModule* sourceShaderModule;


		char shaderCode[8192];
		String text;

		MaterialEditor* parent;
		String& logText;
		bool& bIsCompiling;
		bool bHasBeenModified = false;
		std::chrono::steady_clock::time_point lastModificationTime;
	};





	class MaterialEditor : public SubWindow {
	public:
		friend ShaderEditor;

		MaterialEditor(Material* sourceFile);

	protected:

		virtual void DrawContent(const size_t& imageIndex);


	private:

		void LogText(const String& text);

		std::mutex logTextMutex;
		Material* sourceShaderModule;
		ShaderEditor* vertexShaderEditor;
		ShaderEditor* fragmentShaderEditor;

		bool bIsCompiling;
		String compilationLogs;

		bool bAutoRebuild;
	};
}