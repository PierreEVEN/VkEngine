#pragma once
#include "Vector.h"
#include "Maths.h"
#include <valarray>

struct SRotator
{
	inline SRotator() : x(0), y(0), z(0), w(0) {}
	inline SRotator(const float& inX, const float& inY, const float& inZ, const float& inW) : x(inX), y(inY), z(inZ), w(inW) {}
	inline SRotator(const float& inRoll, const float& inPitch, const float& inYaw) { SetFromEuleurAngles(inRoll, inPitch, inYaw); }
	SRotator(SVector inForwardVector, float inAngle) { SetFromForwardVectorAndAngle(inForwardVector.x, inForwardVector.y, inForwardVector.z, inAngle); }

	inline const float& X() const { return x; }
	inline const float& Y() const { return y; }
	inline const float& Z() const { return z; }
	inline const float& W() const { return w; }

	inline const SVector GetForwardVector() const {
		return SVector(
			1 - 2 * (y * y + z * z),
			2 * (x * y + w * z),
			2 * (x * z - w * y)
		);
	}

	inline const SVector GetRightVector() const {
		return SVector (
			2 * (x * y - w * z),
			1 - 2 * (x * x + z * z),
			2 * (y * z + w * x)
		);
	}

	inline const SVector GetUpVector() const {
		return SVector (
			2 * (x * z + w * y),
			2 * (y * z - w * x),
			1 - 2 * (x * x + y * y)
		);
	}

	inline const float GetRoll() const {
		float sinr_cosp = 2 * (w * x + y * z);
		float cosr_cosp = 1 - 2 * (x * x + y * y);
		return std::atan2(sinr_cosp, cosr_cosp) / G_FPI * 180;
	}

	inline const float GetPitch() const {
		float sinp = 2 * (w * y - z * x);
		if (std::abs(sinp) >= 1)
			return std::copysign(G_FPI / 2, sinp) / G_FPI * 180; // use 90 degrees if out of range
		else
			return std::asin(sinp) / G_FPI * 180;
	}

	inline const float GetYaw() const {
		float siny_cosp = 2 * (w * z + x * y);
		float cosy_cosp = 1 - 2 * (y * y + z * z);
		return std::atan2(siny_cosp, cosy_cosp) / G_FPI * 180;
	}
private:

	inline void SetFromEuleurAngles(const float& inRoll, const float& inPitch, const float& inYaw)
	{
		float cy = cos(((inYaw / 180) * G_FPI) * 0.5f);
		float sy = sin(((inYaw / 180) * G_FPI) * 0.5f);
		float cp = cos(((inPitch / 180) * G_FPI) * 0.5f);
		float sp = sin(((inPitch / 180) * G_FPI) * 0.5f);
		float cr = cos(((inRoll / 180) * G_FPI) * 0.5f);
		float sr = sin(((inRoll / 180) * G_FPI) * 0.5f);

		w = cy * cp * cr + sy * sp * sr;
		x = cy * cp * sr - sy * sp * cr;
		y = sy * cp * sr + cy * sp * cr;
		z = sy * cp * cr - cy * sp * sr;
	}

	inline void SetFromForwardVectorAndAngle(const float& inX, const float& inY, const float& inZ, const float& inAngle)
	{
		float rAngle = (inAngle / 180) * G_FPI;
		float s = sin(rAngle / 2);
		x = inX * s;
		y = inY * s;
		z = inZ * s;
		w = cos(rAngle / 2);
	}


	union
	{
		struct { float x, y, z, w; };
		float coords[4];
	};
};