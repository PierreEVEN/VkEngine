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

		ShaderModule* sourceShaderModule;


		char shaderCode[8192];

		String text;
	};
}