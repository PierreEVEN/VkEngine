#pragma once

#include "SubWindow.h"

namespace Rendering {

	class Texture2D;

	class TextureImportWindow : public SubWindow {
	public:


		TextureImportWindow();

	protected:

		virtual void DrawContent(const size_t& imageIndex) {}

	private:

		void OnPickedTexture(const String& path);
	};
}