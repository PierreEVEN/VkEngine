#pragma once

#include "Constants.h"
#include "SceneComponent.h"

namespace Rendering
{
	class Camera : public SceneComponent
	{
	public:

		Camera() : SceneComponent() {}
		Camera(const STransform& inTransform) : SceneComponent(inTransform) {}

		inline const Mat4f GetViewMatrix() const { return Matrix::MakeLookAtMatrix(GetLocation(), GetLocation() + GetForwardVector(), GetUpVector()); }

		float nearClipPlane = 0.1f;
		float farClipPlane = 1000000.f;
		float fieldOfView = 45.f;
	};
}