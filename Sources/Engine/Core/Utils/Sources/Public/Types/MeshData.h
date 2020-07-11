#pragma once
#include "Vector.h"
#include "Color.h"


struct SVertex
{
	SVertex(const SVector& inLocation)
		: location(inLocation) {}

	SVertex(const SVector& inLocation, const SVector& inNormal)
		: location(inLocation), normal(inNormal) {}

	SVertex(const SVector& inLocation, const SVector2D& inTexcoords)
		: location(inLocation), texcoords(inTexcoords) {}

	SVertex(const SVector& inLocation, const SVector& inNormal, const SVector2D& inTexcoords)
		: location(inLocation), normal(inNormal), texcoords(inTexcoords) {}

	SVertex(const SVector& inLocation, const SVector& inNormal, const SVector2D& inTexcoords, const SVector& inTangent, const SVector& inBitangent, const SLinearColor& inVertexcolor)
		: location(inLocation), normal(inNormal), texcoords(inTexcoords), tangent(inTangent), bitangent(inBitangent), vertexcolor(inVertexcolor) {}

	SVector location;
	SVector normal;
	SVector2D texcoords;
	SVector tangent;
	SVector bitangent;
	SLinearColor vertexcolor;
};