#pragma once

#include "ScenePrimitive.h"

namespace Rendering {

	class StaticMesh;

	class StaticMeshComponent : public ScenePrimitive {
	public:


		StaticMeshComponent(const STransform& inTransform, StaticMesh* meshLink);

		virtual void PreDraw(ViewportInstance* inViewport, const size_t& inImageIndex);
		virtual void Draw(VkCommandBuffer& inCommandBuffer, ViewportInstance* viewport, const size_t& imageIndex);

	private:

		StaticMesh* staticMesh;
	};
}