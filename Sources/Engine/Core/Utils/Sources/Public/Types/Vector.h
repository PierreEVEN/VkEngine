#pragma once

#include <stdint.h>

enum class EVectorAxis
{
	AXIS_X = 0,
	AXIS_Y = 1,
	AXIS_Z = 2
};

template<typename T>
struct IVector
{
	IVector() : x(0), y(0), z(0) {}
	IVector(const T& value) : x(value), y(value), z(value) {}
	IVector(const T& inX, const T& inY, const T& inZ) : x(inX), y(inY), z(inZ) {}

	inline const T& X() const { return x; }
	inline const T& Y() const { return y; }
	inline const T& Z() const { return z; }

	inline void SetX(const T& value) { x = value; }
	inline void SetY(const T& value) { y = value; }
	inline void SetZ(const T& value) { z = value; }

	inline const bool operator==(const IVector<T>& other) const	{
		return x == other.x && y == other.y && z == other.z;
	}
	inline const bool operator!=(const IVector<T>& other) const	{
		return x != other.x || y != other.y || z != other.z;
	}

	/* [] operator */
	inline const IVector<T> operator[](const EVectorAxis& axis) const {
		return coords[axis];
	}

	/* X IVector Operators */
	inline const IVector<T> operator+(const IVector<T>& other) const {
		return IVector<T>(x + other.x, y + other.y, z + other.z);
	}
	inline const IVector<T> operator-(const IVector<T>& other) const {
		return IVector<T>(x - other.x, y - other.y, z - other.z);
	}
	inline const IVector<T> operator*(const IVector<T>& other) const {
		return IVector<T>(x * other.x, y * other.y, z * other.z);
	}
	inline const IVector<T> operator/(const IVector<T>& other) const {
		return IVector<T>(x / other.x, y / other.y, z / other.z);
	}

	/* X= IVector Operators */
	inline const IVector<T> operator+=(const IVector<T>& other) {
		return IVector<T>(x += other.x, y += other.y, z += other.z);
	}
	inline const IVector<T> operator-=(const IVector<T>& other) {
		return IVector<T>(x -= other.x, y -= other.y, z -= other.z);
	}
	inline const IVector<T> operator*=(const IVector<T>& other) {
		return IVector<T>(x *= other.x, y *= other.y, z *= other.z);
	}
	inline const IVector<T> operator/=(const IVector<T>& other) {
		return IVector<T>(x /= other.x, y /= other.y, z /= other.z);
	}


	/* X T Operators */
	inline const IVector<T> operator+(const T& other) const {
		return IVector<T>(x + other, y + other, z + other);
	}
	inline const IVector<T> operator-(const T& other) const {
		return IVector<T>(x - other, y - other, z - other);
	}
	inline const IVector<T> operator*(const T& other) const {
		return IVector<T>(x * other, y * other, z * other);
	}
	inline const IVector<T> operator/(const T& other) const {
		return IVector<T>(x / other, y / other, z / other);
	}


	/* X= T Operators */
	inline const IVector<T> operator+=(const T& other) {
		return IVector<T>(x += other, y += other, z += other);
	}
	inline const IVector<T> operator-=(const T& other) {
		return IVector<T>(x -= other, y -= other, z -= other);
	}
	inline const IVector<T> operator*=(const T& other) {
		return IVector<T>(x *= other, y *= other, z *= other);
	}
	inline const IVector<T> operator/=(const T& other) {
		return IVector<T>(x /= other, y /= other, z /= other);
	}

private:
	union
	{
		struct	{
			T x, y, z;
		};

		T coords[3];
	};
};

template<typename T>
struct IVector2D
{
	IVector2D() : x(0), y(0) {}
	IVector2D(const T& value) : x(value), y(value) {}
	IVector2D(const T& inX, const T& inY) : x(inX), y(inY) {}

	inline const T& X() const { return x; }
	inline const T& Y() const { return y; }

	inline void SetX(const T& value) { x = value; }
	inline void SetY(const T& value) { y = value; }

	inline const bool operator==(const IVector<T>& other) const {
		return x == other.x && y == other.y;
	}
	inline const bool operator!=(const IVector<T>& other) const {
		return x != other.x || y != other.y;
	}

	/* X IVector Operators */
	inline const IVector<T> operator+(const IVector<T>& other) const {
		return IVector<T>(x + other.x, y + other.y);
	}
	inline const IVector<T> operator-(const IVector<T>& other) const {
		return IVector<T>(x - other.x, y - other.y);
	}
	inline const IVector<T> operator*(const IVector<T>& other) const {
		return IVector<T>(x * other.x, y * other.y);
	}
	inline const IVector<T> operator/(const IVector<T>& other) const {
		return IVector<T>(x / other.x, y / other.y);
	}


	/* X= IVector Operators */
	inline const IVector<T> operator+=(const IVector<T>& other) {
		return IVector<T>(x += other.x, y += other.y);
	}
	inline const IVector<T> operator-=(const IVector<T>& other) {
		return IVector<T>(x -= other.x, y -= other.y);
	}
	inline const IVector<T> operator*=(const IVector<T>& other) {
		return IVector<T>(x *= other.x, y *= other.y);
	}
	inline const IVector<T> operator/=(const IVector<T>& other) {
		return IVector<T>(x /= other.x, y /= other.y);
	}


	/* X T Operators */
	inline const IVector<T> operator+(const T& other) const {
		return IVector<T>(x + other, y + other);
	}
	inline const IVector<T> operator-(const T& other) const {
		return IVector<T>(x - other, y - other);
	}
	inline const IVector<T> operator*(const T& other) const {
		return IVector<T>(x * other, y * other);
	}
	inline const IVector<T> operator/(const T& other) const {
		return IVector<T>(x / other, y / other);
	}


	/* X= T Operators */
	inline const IVector<T> operator+=(const T& other) {
		return IVector<T>(x += other, y += other);
	}
	inline const IVector<T> operator-=(const T& other) {
		return IVector<T>(x -= other, y -= other);
	}
	inline const IVector<T> operator*=(const T& other) {
		return IVector<T>(x *= other, y *= other);
	}
	inline const IVector<T> operator/=(const T& other) {
		return IVector<T>(x /= other, y /= other);
	}

private:
	T x, y;
};

typedef IVector<float> SVector;
typedef IVector<double> SVectorDouble;
typedef IVector<int32_t> SIntVector;
typedef IVector<int64_t> SLongVector;

typedef IVector2D<float> SVector2D;
typedef IVector2D<double> SVectorDouble2D;
typedef IVector2D<int32_t> SIntVector2D;
typedef IVector2D<int64_t> SLongVector2D;