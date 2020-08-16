#pragma once

#include "Constants.h"

namespace Rendering
{
	class SceneComponent
	{
	public:

		SceneComponent() {}
		SceneComponent(const STransform& inTransform) : componentTransform(inTransform) {}
		virtual ~SceneComponent() {}

		inline void AddWorldOffset(const SVector& inOffset) { componentTransform.location += inOffset; }
		inline void AddLocalOffset(const SVector& inOffset) {
			componentTransform.location +=
				componentTransform.rotation.GetForwardVector() * inOffset.x +
				componentTransform.rotation.GetRightVector() * inOffset.y +
				componentTransform.rotation.GetUpVector() * inOffset.z;
		}

		inline const SVector& GetLocation() const { return componentTransform.location; }
		inline const SQuatf& GetRotation() const { return componentTransform.rotation; }
		inline const SVector& GetScale() const { return componentTransform.scale3d; }
		inline const STransform& GetTransform() const { return componentTransform; }

		inline const SVector GetForwardVector() const { return componentTransform.rotation.GetForwardVector(); }
		inline const SVector GetRightVector() const { return componentTransform.rotation.GetRightVector(); }
		inline const SVector GetUpVector() const { return componentTransform.rotation.GetUpVector(); }

		inline void SetLocation(const SVector& location) { componentTransform.location = location; }
		inline void SetRotation(const SQuatf& rotation) { componentTransform.rotation = rotation; }
		inline void SetScale(const SVector& scale) { componentTransform.scale3d = scale; }
		inline void SetTransform(const STransform& transform) { componentTransform = transform; }

	private:
		STransform componentTransform;
	};
}