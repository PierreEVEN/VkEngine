#pragma once

#include <stdint.h>
#include "String.h"

enum class EVectorAxis
{
	AXIS_X = 0,
	AXIS_Y = 1,
	AXIS_Z = 2
};

template<typename T>
struct IVector : public IStringable
{
	IVector() : x(0), y(0), z(0) {}
	IVector(const T& value) : x(value), y(value), z(value) {}
	IVector(const T& inX, const T& inY, const T& inZ) : x(inX), y(inY), z(inZ) {}

	inline virtual const T& X() const { return x; }
	inline virtual const T& Y() const { return y; }
	inline virtual const T& Z() const { return z; }

	inline virtual void SetX(const T& value) { x = value; }
	inline virtual void SetY(const T& value) { y = value; }
	inline virtual void SetZ(const T& value) { z = value; }

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


	virtual String ToString() const override
	{
		return String(String("(") + String(x) +	", " + String(y) + ", " + String(z) + ")");
	}

private:
	union
	{
		struct	{
			alignas(8) T x, y, z;
		};

		T coords[3];
	};
};

template<typename T>
struct IVector2D : public IStringable
{
	IVector2D() : x(0), y(0) {}
	IVector2D(const T& value) : x(value), y(value) {}
	IVector2D(const T& inX, const T& inY) : x(inX), y(inY) {}

	inline virtual const T& X() const { return x; }
	inline virtual const T& Y() const { return y; }

	inline virtual void SetX(const T& value) { x = value; }
	inline virtual void SetY(const T& value) { y = value; }

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


	virtual String ToString() const override
	{
		return String(String("(") + String(x) + ", " + String(x) + ")");
	}

private:
	alignas(8) T x, y;
};

struct SVector : public IVector<float>
{
	using IVector::IVector;
};

struct SVectorDouble : public IVector<double>
{
	using IVector::IVector;
};

struct SIntVector : public IVector<int32_t>
{
	using IVector::IVector;
};

struct SLongVector : public IVector<int64_t>
{
	using IVector::IVector;
};

struct SVector2D : public IVector2D<float>
{
	using IVector2D::IVector2D;
};

struct SVectorDouble2D : public IVector2D<double>
{
	using IVector2D::IVector2D;
};

struct SIntVector2D : public IVector2D<int32_t>
{
	using IVector2D::IVector2D;
};

struct SLongVector2D : public IVector2D<int64_t>
{
	using IVector2D::IVector2D;
};