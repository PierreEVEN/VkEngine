#pragma once

#include "SwapChain.h"

namespace Rendering
{
	class ViewportInstance
	{
	public:
		ViewportInstance();
		~ViewportInstance();

		inline const VkExtent2D& GetViewportExtend() const { return viewportSwapChain->GetSwapChainExtend(); }
		inline const uint32_t& GetViewportWidth() const { return viewportSwapChain->GetSwapChainExtend().width; }
		inline const uint32_t& GetViewportHeight() const { return viewportSwapChain->GetSwapChainExtend().height; }

	private:

		SwapChain* viewportSwapChain;
	};
}