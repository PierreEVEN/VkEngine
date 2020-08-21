#pragma once

#include "SubWindow.h"

namespace Rendering {
	class ContentBrowser : public SubWindow {
	public:

		ContentBrowser() : SubWindow("Content browser") {}

	protected:

		virtual ~ContentBrowser() {}

		virtual void DrawContent(const size_t& imageIndex);

	};
}