#pragma once

#include "SubWindow.h"

namespace Rendering {
	class ShaderModule;

	class ShaderCodeEditor : public SubWindow {
	public:

		ShaderCodeEditor(ShaderModule* sourceFile);

	protected:

		virtual void DrawContent(const size_t& imageIndex);


	private:

		inline void OnSuccessfullyCompiledShader() { compilationLogs << "compilation succeed" << String::ENDL; }
		inline void OnShaderCompilationError(const String& error) { compilationLogs << error << String::ENDL; }
		ShaderModule* sourceShaderModule;


		char shaderCode[8192];
		String text;

		String compilationLogs;
		bool bHasBeenModified = false;
		std::chrono::steady_clock::time_point lastModificationTime;
	};
}