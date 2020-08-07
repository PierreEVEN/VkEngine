#pragma once

#include "GraphicRessource.h"

namespace Rendering
{
	struct Texture2D : public Ressource
	{
		static VkImageView CreateImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels);
	};
}