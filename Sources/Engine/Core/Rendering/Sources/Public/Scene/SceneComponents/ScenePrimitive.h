#pragma once

#include "SceneComponent.h"

namespace Rendering {

	class StaticMesh;
	class ViewportInstance;

	class ScenePrimitive : public SceneComponent {
	public:
		ScenePrimitive(const STransform& inTransform);

		virtual void Draw(VkCommandBuffer& inCommandBuffer, ViewportInstance* viewport, const size_t& imageIndex) = 0;

	};
}