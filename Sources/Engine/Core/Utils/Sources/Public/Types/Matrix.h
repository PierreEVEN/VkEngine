#pragma once
#include "Vector.h"
#include "Types/Rotator.h"

template<typename T>
struct IMatrix3
{
	inline IMatrix3() : IMatrix3(1) {}
	inline IMatrix3(const T& scalar) :
		x1(scalar), x2(0), x3(0),
		y1(0), y2(scalar), y3(0),
		z1(0), z2(0), z3(scalar)
	{ }

	inline const IVector3<T>& operator[](const size_t& elem) const { return rows[elem]; }

	union
	{
		struct {
			T
				x1, x2, x3,
				y1, y2, y3,
				z1, z2, z3;
		};
		struct { IVector3<T> 
			a,
			b,
			c;
		};
		IVector3<T> rows[3];
		T coords[9];
	};
};


template<typename T>
struct IMatrix4
{
	inline IMatrix4() : IMatrix4(1) {}
	inline IMatrix4(const T& scalar) :
		x1(scalar), x2(0), x3(0), x4(0),
		y1(0), y2(scalar), y3(0), y4(0),
		z1(0), z2(0), z3(scalar), z4(0),
		w1(0), w2(0), w3(0), w4(scalar)
	{}
	inline IMatrix4(const IQuaternion<T>& quat)
		: IMatrix4(1)
	{
		Rotate(quat);
	}

	inline IMatrix4(const IVector3<T>& pos, const IQuaternion<T>& rot, const IVector3<T>& scale)
		: IMatrix4(1)
	{
		Translate(pos);
		Rotate(rot);
		Scale(scale);
	}

	inline void Translate(const IVector3<T>& translation) {
		d = a * translation.x + b * translation.y + c * translation.z + d;
	}

	inline void Rotate(const IQuaternion<T>& quat) {
		const T qxx(quat.x * quat.x);
		const T qyy(quat.y * quat.y);
		const T qzz(quat.z * quat.z);
		const T qxz(quat.x * quat.z);
		const T qxy(quat.x * quat.y);
		const T qyz(quat.y * quat.z);
		const T qwx(quat.w * quat.x);
		const T qwy(quat.w * quat.y);
		const T qwz(quat.w * quat.z);

		a.x = T(1) - T(2) * (qyy + qzz);
		a.y = T(2) * (qxy + qwz);
		a.z = T(2) * (qxz - qwy);

		b.x = T(2) * (qxy - qwz);
		b.y = T(1) - T(2) * (qxx + qzz);
		b.z = T(2) * (qyz + qwx);

		c.x = T(2) * (qxz + qwy);
		c.y = T(2) * (qyz - qwx);
		c.z = T(1) - T(2) * (qxx + qyy);
	}

	inline void Scale(const IVector3<T>& scale) {
		a = a * scale.x;
		b = b * scale.y;
		c = c * scale.z;
		d = d;
	}


	inline IVector4<T>& operator[](const size_t& elem) { return rows[elem]; }

	inline const IMatrix4<T> operator *(const IMatrix4<T>& other) {
		const IVector4<T> oa = other.a;
		const IVector4<T> ob = other.b;
		const IVector4<T> oc = other.c;
		const IVector4<T> od = other.d;

		IMatrix4<T> result;
		result.a = a * oa.x + b * oa.y + c * oa.z + d * oa.w;
		result.b = a * ob.x + b * ob.y + c * ob.z + d * ob.w;
		result.c = a * oc.x + b * oc.y + c * oc.z + d * oc.w;
		result.d = a * od.x + b * od.y + c * od.z + d * od.w;
		return result;
	}

	union
	{
		struct {
			T
				x1, x2, x3, x4,
				y1, y2, y3, y4,
				z1, z2, z3, z4,
				w1, w2, w3, w4;
		};
		struct { IVector4<T> 
			a,
			b,
			c,
			d;
		};

		IVector4<T> rows[4];
		T coords[16];
	};
};

typedef IMatrix3<float> Mat3f;
typedef IMatrix3<double> Mat3d;
typedef IMatrix4<float> Mat4f;
typedef IMatrix4<double> Mat4d;



namespace Matrix
{
	inline const Mat4f MakePerspectiveMatrix(float fov, float aspectRatio, float zNear, float zFar) {
		float const tanHalfFovy = tan(fov / 2.f);

		Mat4f Result(0.f);
		Result.a.x = 1.f / (aspectRatio * tanHalfFovy);
		Result.b.y = 1.f / (tanHalfFovy);
		Result.c.z = -(zFar + zNear) / (zFar - zNear);
		Result.c.w = -1.f;
		Result.d.z = -(2.f * zFar * zNear) / (zFar - zNear);
		return Result;
	}

	inline const Mat4f MakeLookAtMatrix(const SVector& cameraLocation, const SVector& viewTarget, const SVector& upVector)
	{
		SVector const f(SVector::Normalize(viewTarget - cameraLocation));
		SVector const s(SVector::Normalize(SVector::Cross(f, upVector)));
		SVector const u(SVector::Cross(s, f));

		Mat4f Result(1);
		Result[0][0] = s.x;
		Result[1][0] = s.y;
		Result[2][0] = s.z;
		Result[0][1] = u.x;
		Result[1][1] = u.y;
		Result[2][1] = u.z;
		Result[0][2] = -f.x;
		Result[1][2] = -f.y;
		Result[2][2] = -f.z;
		Result[3][0] = -SVector::Dot(s, cameraLocation);
		Result[3][1] = -SVector::Dot(u, cameraLocation);
		Result[3][2] = SVector::Dot(f, cameraLocation);
		return Result;
	}

}
