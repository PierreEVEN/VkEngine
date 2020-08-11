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

	inline const T Length() const { return sqrt(x * x + y * y + z * z); }

	inline const T& GetMax() const {
		if (x > y && x > z)
			return x;
		else if (y > x && y > z)
			return y;
		else
			return z;
	}

	inline const T& GetMin() const
	{
		if (x < y && x < z)
			return x;
		else if (y < x && y < z)
			return y;
		else
			return z;
	}

	inline void Normalize()	{
		T length = Length();
		x /= length;
		y /= length;
		z /= length;
	}

	inline static const IVector<T> Normalize(const IVector<T> inVector) {
		double length = inVector.Length();
		return IVector<T>(inVector.x / length, inVector.y / length, inVector.z / length)
	}

	inline static const IVector<T> Cross(const IVector<T>& a, const IVector<T>& b) {
		return IVector<T>(
			a.y * b.z - a.z * b.y,
			a.z * b.x - a.x * b.z,
			a.x * b.y - a.y * b.x
			);
	}

	inline static const T Dot(const IVector<T>& a, const IVector<T>& b) {
		return a.x * b.x + a.y * b.y + a.z * b.z;
	}

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

	inline const bool operator==(const IVector<T>& other) const {
		return x == other.x && y == other.y;
	}
	inline const bool operator!=(const IVector<T>& other) const {
		return x != other.x || y != other.y;
	}


	/* [] operator */
	inline const IVector<T> operator[](const EVectorAxis& axis) const {
		return coords[axis];
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

	inline const T Length() const { return sqrt(x * x + y * y); }

	inline const T& GetMax() const {
		if (x > y)
			return x;
		return y;
	}

	inline const T& GetMin() const
	{
		if (x < y)
			return x;
		return y;
	}

	inline void Normalize() {
		T length = Length();
		x /= length;
		y /= length;
	}

	inline static const IVector2D<T> Normalize(const IVector2D<T> inVector) {
		double length = inVector.Length();
		return IVector2D<T>(inVector.x / length, inVector.y / length, inVector.z / length)
	}

	inline static const T Dot(const IVector2D<T>& a, const IVector2D<T>& b) {
		return a.x * b.x + a.y * b.y;
	}


	union {
		struct {
			T x, y;
		};
		T coords[2];
	};
};

typedef IVector<float> SVector;
typedef IVector<double> SVectorDouble;
typedef IVector<int32_t> SIntVector;
typedef IVector<int64_t> SLongVector;

typedef IVector2D<float> SVector2D;
typedef IVector2D<double> SVectorDouble2D;
typedef IVector2D<int32_t> SIntVector2D;
typedef IVector2D<int64_t> SLongVector2D;