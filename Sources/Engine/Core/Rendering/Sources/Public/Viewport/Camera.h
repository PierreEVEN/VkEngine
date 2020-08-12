#pragma once

#include "Constants.h"

namespace Rendering
{
	class Camera
	{
	public:

		inline const glm::mat4 GetViewMatrix() const { return glm::lookAt(GetPosition(), GetPosition() + glm::make_vec3(rotation.GetForwardVector().coords), glm::make_vec3(rotation.GetUpVector().coords)); }
		inline const glm::vec3 GetPosition() const { return glm::vec4(location.x, location.y, location.z, 0); }

		float nearClipPlane = 0.1f;
		float farClipPlane = 1000000.f;
		SVector location;
		SRotator rotation;
		float fieldOfView = 45.f;
	};
}