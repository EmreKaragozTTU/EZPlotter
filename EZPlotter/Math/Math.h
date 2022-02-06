#pragma once

#include <cmath>
#include <algorithm>
#include <xmmintrin.h>

#include "Common.h"

namespace Math
{

	const F32 PI = 3.14159265f;

	inline F32 ToRad(F32 a);

	inline F32 ToDeg(F32 a);

	inline F32 Clamp(F32 n, F32 minRange, F32 maxRange);

	inline F32 Saturate(F32 n);

	template<typename T, U32 n>
	struct Vector
	{
		T Elements[n];
	};

	template<>
	struct Vector<F32, 2>
	{
		Vector()
		{

		};

		Vector(F32 x, F32 y)
		{
			Elements[0] = x;
			Elements[1] = y;
		};

		union
		{
			F32 Elements[2];

			struct
			{
				F32 x, y;
			};
		};
	};

	typedef Vector<F32, 2> Vector2;


	template<>
	struct Vector<F32, 3>
	{
		Vector()
		{

		};

		Vector(F32 x, F32 y, F32 z)
		{
			Elements[0] = x;
			Elements[1] = y;
			Elements[2] = z;
		};

		union
		{
			F32 Elements[3];

			struct
			{
				F32 x, y, z;
			};

			struct
			{
				F32 r, g, b;
			};
		};
	};

	typedef Vector<F32, 3> Vector3;

	

	typedef Vector<F32, 3> ColorRGB;

	Vector3 operator+(const Vector3& v0, const Vector3& v1);

	Vector3 operator-(const Vector3& v0, const Vector3& v1);

	Vector3 operator*(const Vector3& v0, F32 f);

	Vector3 operator/(const Vector3& v0, F32 f);

	F32 Dot(const Vector3& v0, const Vector3& v1);

	Vector3 Cross(const Vector3& v0, const Vector3& v1);

	Vector3 Normalize(const Vector3& v0);

	F32 Length(const Vector3& v0);

	template<>
	struct Vector<F32, 4>
	{
		Vector()
		{

		};

		Vector(F32 x, F32 y, F32 z, F32 w)
		{
			Elements[0] = x;
			Elements[1] = y;
			Elements[2] = z;
			Elements[3] = w;
		};

		union
		{
			F32 Elements[4];

			struct
			{
				F32 x, y, z, w;
			};

			struct
			{
				F32 r, g, b, a;
			};
		};
	};

	typedef Vector<F32, 4> Vector4;
	typedef Vector<F32, 4> ColorRGBA;

	Vector4 operator+(const Vector4& v0, const Vector4& v1);

	Vector4 operator-(const Vector4& v0, const Vector4& v1);

	Vector4 operator*(const Vector4& v0, F32 f);

	Vector4 operator/(const Vector4& v0, F32 f);

	F32 Dot(const Vector4& v0, const Vector4& v1);

	Vector4 Normalize(const Vector4& v0);

	F32 Length(const Vector4& v0);


	template <typename T, U32 dimX, U32 dimY>
	struct Matrix
	{
		T Elements[dimX][dimY];
	};

	template <>
	struct Matrix<F32, 3, 3>
	{
		union
		{
			F32 Elements[3][3];

			struct
			{
				F32 m00, m01, m02;
				F32 m10, m11, m12;
				F32 m20, m21, m22;
			};
		};
	};

	typedef Matrix<F32, 3, 3> Matrix3x3;


	template <>
	struct Matrix<F32, 4, 4>
	{
		union
		{
			F32 Elements[4][4];

			struct
			{
				F32 m00, m01, m02, m03;
				F32 m10, m11, m12, m13;
				F32 m20, m21, m22, m23;
				F32 m30, m31, m32, m33;
			};
		};
	};

	typedef Matrix<F32, 4, 4> Matrix4x4;

	Matrix4x4 operator*(const Matrix4x4& mat0, const Matrix4x4& mat1);

	Vector4 operator*(const Matrix4x4& mat0, const Vector4& v0);

	Matrix4x4 Transpose(const Matrix4x4& mat0);

	Matrix4x4 Translation(F32 Tx, F32 Ty, F32 Tz);

	Matrix4x4 Scale(F32 Sx, F32 Sy, F32 Sz);

	Matrix4x4 RotationX(F32 deg);

	Matrix4x4 RotationY(F32 deg);

	Matrix4x4 RotationZ(F32 deg);

	Matrix4x4 Rotation(const Vector3& v0, F32 deg);

	Matrix4x4 Perspective_GL(F32 fovDeg, F32 aspect, F32 zNear, F32 zFar);

	Matrix4x4 PerspectiveFovMatrix(F32 FovYDeg, F32 AspectRatio, F32 ZNear, F32 ZFar);
	Matrix4x4 PerspectiveFovMatrixLH(F32 FovYDeg, F32 AspectRatio, F32 ZNear, F32 ZFar);

	Matrix4x4 LookAtMatrix(const Vector3& Eye, const Vector3 &Target, const Vector3& Up);
	Matrix4x4 LookAtMatrixLH(const Vector3& Eye, const Vector3 &Target, const Vector3& Up);
}