#pragma once

namespace Maths
{

	template<typename T>
	inline const T GetMax(const T& inA, const T& inB) {
		return inA > inB ? inA : inB;
	}

	template<typename T>
	inline const T GetMin(const T& inA, const T& inB) {
		return inA < inB ? inA : inB;
	}

	template<typename T>
	inline const T Clamp(const T& inValue, const T& max, const T& min) {
		return inValue > max ? max : inValue < min ? min : inValue;
	}
}