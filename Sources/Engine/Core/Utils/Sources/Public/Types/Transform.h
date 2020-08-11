#pragma once
#include "Vector.h"
#include "Rotator.h"

struct STransform
{
	STransform() {}
	STransform(const SVector& inLocation) : location(inLocation) {}
	STransform(const SRotator& inRotation) : rotation(inRotation) {}
	STransform(const SVector& inLocation, const SVector& inScale3d) : location(inLocation), scale3d(inScale3d) {}
	STransform(const SVector& inLocation, const SRotator& inRotation) : location(inLocation), rotation(inRotation) {}
	STransform(const SVector& inLocation, const SRotator& inRotation, const SVector& inScale3d) : location(inLocation), rotation(inRotation), scale3d(inScale3d) {}

	inline SVector& Position() { return location; }
	inline SRotator& Rotation() { return rotation; }
	inline SVector& Scale() { return scale3d; }

	inline void SetPosition(const SVector& inLocation) { location = inLocation; }
	inline void SetRotation(const SRotator& inRotator) { rotation = inRotator; }
	inline void SetScale(const SVector& inScale3D) { scale3d = inScale3D; }

private:
	SVector location;
	SVector scale3d;
	SRotator rotation;
};