#pragma once

#include "Constants.h"

namespace Rendering
{
	void LinkValidationLayers(VkInstanceCreateInfo& createInfos);

	void CreateValidationLayers();
	void DestroyValidationLayers();
}