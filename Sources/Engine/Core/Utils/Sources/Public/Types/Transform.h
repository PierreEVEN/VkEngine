#pragma once
#include "Vector.h"
#include "Rotator.h"

struct STransform
{
	STransform() {}
	STransform(const SVector& inLocation) : location(inLocation) {}
	STransform(const SQuatf& inRotation) : rotation(inRotation) {}
	STransform(const SVector& inLocation, const SVector& inScale3d) : location(inLocation), scale3d(inScale3d) {}
	STransform(const SVector& inLocation, const SQuatf& inRotation) : location(inLocation), rotation(inRotation) {}
	STransform(const SVector& inLocation, const SQuatf& inRotation, const SVector& inScale3d) : location(inLocation), rotation(inRotation), scale3d(inScale3d) {}

	SVector location;
	SVector scale3d;
	SQuatf rotation;
};