#pragma once

#include <stdint.h>
#include "String.h"

template<typename T>
struct IVector3
{
	IVector3() : x(0), y(0), z(0) {}
	IVector3(const T& value) : x(value), y(value), z(value) {}
	IVector3(const T& inX, const T& inY, const T& inZ) : x(inX), y(inY), z(inZ) {}

	inline const bool operator==(const IVector3<T>& other) const	{
		return x == other.x && y == other.y && z == other.z;
	}
	inline const bool operator!=(const IVector3<T>& other) const	{
		return x != other.x || y != other.y || z != other.z;
	}

	/* [] operator */
	inline T& operator[](const size_t& axis) {
		return coords[axis];
	}

	/* X IVector Operators */
	inline const IVector3<T> operator+(const IVector3<T>& other) const {
		return IVector3<T>(x + other.x, y + other.y, z + other.z);
	}
	inline const IVector3<T> operator-(const IVector3<T>& other) const {
		return IVector3<T>(x - other.x, y - other.y, z - other.z);
	}
	inline const IVector3<T> operator*(const IVector3<T>& other) const {
		return IVector3<T>(x * other.x, y * other.y, z * other.z);
	}
	inline const IVector3<T> operator/(const IVector3<T>& other) const {
		return IVector3<T>(x / other.x, y / other.y, z / other.z);
	}

	/* X= IVector Operators */
	inline const IVector3<T> operator+=(const IVector3<T>& other) {
		return IVector3<T>(x += other.x, y += other.y, z += other.z);
	}
	inline const IVector3<T> operator-=(const IVector3<T>& other) {
		return IVector3<T>(x -= other.x, y -= other.y, z -= other.z);
	}
	inline const IVector3<T> operator*=(const IVector3<T>& other) {
		return IVector3<T>(x *= other.x, y *= other.y, z *= other.z);
	}
	inline const IVector3<T> operator/=(const IVector3<T>& other) {
		return IVector3<T>(x /= other.x, y /= other.y, z /= other.z);
	}


	/* X T Operators */
	inline const IVector3<T> operator+(const T& other) const {
		return IVector3<T>(x + other, y + other, z + other);
	}
	inline const IVector3<T> operator-(const T& other) const {
		return IVector3<T>(x - other, y - other, z - other);
	}
	inline const IVector3<T> operator*(const T& other) const {
		return IVector3<T>(x * other, y * other, z * other);
	}
	inline const IVector3<T> operator/(const T& other) const {
		return IVector3<T>(x / other, y / other, z / other);
	}


	/* X= T Operators */
	inline const IVector3<T> operator+=(const T& other) {
		return IVector3<T>(x += other, y += other, z += other);
	}
	inline const IVector3<T> operator-=(const T& other) {
		return IVector3<T>(x -= other, y -= other, z -= other);
	}
	inline const IVector3<T> operator*=(const T& other) {
		return IVector3<T>(x *= other, y *= other, z *= other);
	}
	inline const IVector3<T> operator/=(const T& other) {
		return IVector3<T>(x /= other, y /= other, z /= other);
	}

	inline operator String() const {
		return '{' + ToString(x) + ", " + ToString(y) + ", " + ToString(z) + '}';
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

	inline IVector3<T>& Normalize()	{
		T length = Length();
		x /= length;
		y /= length;
		z /= length;
		return *this;
	}

	inline static const IVector3<T> Normalize(const IVector3<T> inVector) {
		T length = inVector.Length();
		return IVector3<T>(inVector.x / length, inVector.y / length, inVector.z / length);
	}

	inline static const IVector3<T> Cross(const IVector3<T>& a, const IVector3<T>& b) {
		return IVector3<T>(
			a.y * b.z - a.z * b.y,
			a.z * b.x - a.x * b.z,
			a.x * b.y - a.y * b.x
			);
	}

	inline static const T Dot(const IVector3<T>& a, const IVector3<T>& b) {
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
struct IVector2
{
	IVector2() : x(0), y(0) {}
	IVector2(const T& value) : x(value), y(value) {}
	IVector2(const T& inX, const T& inY) : x(inX), y(inY) {}

	inline const bool operator==(const IVector2<T>& other) const {
		return x == other.x && y == other.y;
	}
	inline const bool operator!=(const IVector2<T>& other) const {
		return x != other.x || y != other.y;
	}

	inline operator String() const {
		return '{' + ToString(x) + ", " + ToString(y) + '}';
	}

	/* [] operator */
	inline T& operator[](const size_t& axis) {
		return coords[axis];
	}

	/* X IVector Operators */
	inline const IVector3<T> operator+(const IVector3<T>& other) const {
		return IVector3<T>(x + other.x, y + other.y);
	}
	inline const IVector3<T> operator-(const IVector3<T>& other) const {
		return IVector3<T>(x - other.x, y - other.y);
	}
	inline const IVector3<T> operator*(const IVector3<T>& other) const {
		return IVector3<T>(x * other.x, y * other.y);
	}
	inline const IVector3<T> operator/(const IVector3<T>& other) const {
		return IVector3<T>(x / other.x, y / other.y);
	}


	/* X= IVector Operators */
	inline const IVector3<T> operator+=(const IVector3<T>& other) {
		return IVector3<T>(x += other.x, y += other.y);
	}
	inline const IVector3<T> operator-=(const IVector3<T>& other) {
		return IVector3<T>(x -= other.x, y -= other.y);
	}
	inline const IVector3<T> operator*=(const IVector3<T>& other) {
		return IVector3<T>(x *= other.x, y *= other.y);
	}
	inline const IVector3<T> operator/=(const IVector3<T>& other) {
		return IVector3<T>(x /= other.x, y /= other.y);
	}


	/* X T Operators */
	inline const IVector3<T> operator+(const T& other) const {
		return IVector3<T>(x + other, y + other);
	}
	inline const IVector3<T> operator-(const T& other) const {
		return IVector3<T>(x - other, y - other);
	}
	inline const IVector3<T> operator*(const T& other) const {
		return IVector3<T>(x * other, y * other);
	}
	inline const IVector3<T> operator/(const T& other) const {
		return IVector3<T>(x / other, y / other);
	}


	/* X= T Operators */
	inline const IVector3<T> operator+=(const T& other) {
		return IVector3<T>(x += other, y += other);
	}
	inline const IVector3<T> operator-=(const T& other) {
		return IVector3<T>(x -= other, y -= other);
	}
	inline const IVector3<T> operator*=(const T& other) {
		return IVector3<T>(x *= other, y *= other);
	}
	inline const IVector3<T> operator/=(const T& other) {
		return IVector3<T>(x /= other, y /= other);
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

	inline static const IVector2<T> Normalize(const IVector2<T> inVector) {
		double length = inVector.Length();
		return IVector2<T>(inVector.x / length, inVector.y / length, inVector.z / length)
	}

	inline static const T Dot(const IVector2<T>& a, const IVector2<T>& b) {
		return a.x * b.x + a.y * b.y;
	}


	union {
		struct {
			T x, y;
		};
		T coords[2];
	};
};


template<typename T>
struct IVector4
{
	inline IVector4() : x(0), y(0), z(0), w(0) {}
	inline IVector4(const T& scalar) : x(scalar), y(scalar), z(scalar), w(scalar) {}
	inline IVector4(const T& inx, const T& iny, const T& inz, const T& inw) : x(inx), y(iny), z(inz), w(inw) {}

	/** Operator + */
	inline const IVector4<T> operator+(const IVector4<T>& other) const {
		return IVector4<T>(x + other.x, y + other.y, z + other.z, w + other.w);
	}
	inline const IVector4<T> operator+(const IVector3<T>& other) const {
		return IVector4<T>(x + other.x, y + other.y, z + other.z, w);
	}
	inline const IVector4<T> operator+(const T& scalar) const {
		return IVector4<T>(x + scalar, y + scalar, z + scalar, w + scalar);
	}

	/** Operator * */
	inline const IVector4<T> operator*(const IVector4<T>& other) const {
		return IVector4<T>(x * other.x, y * other.y, z * other.z, w * other.w);
	}
	inline const IVector4<T> operator*(const IVector3<T>& other) const {
		return IVector4<T>(x * other.x, y * other.y, z * other.z, w);
	}
	inline const IVector4<T> operator*(const T& scalar) const {
		return IVector4<T>(x * scalar, y * scalar, z * scalar, w * scalar);
	}

	/* [] operator */
	inline T& operator[](const size_t& axis) {
		return coords[axis];
	}

	union
	{
		struct {
			T x, y, z, w;
		};

		T coords[4];
	};
};

typedef IVector3<float> SVector;
typedef IVector3<double> SVectorDouble;
typedef IVector3<int32_t> SIntVector;
typedef IVector3<int64_t> SLongVector;

typedef IVector2<float> SVector2D;
typedef IVector2<double> SVectorDouble2D;
typedef IVector2<int32_t> SIntVector2D;
typedef IVector2<int64_t> SLongVector2D;