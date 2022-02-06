#include "Math.h"

namespace Math
{

	inline F32 DegToRad(F32 a)
	{
		return a * (PI / 360.0f);
	}

	inline F32 RadToDeg(F32 a)
	{
		return a * (360.0f / PI);
	}

	inline F32 Clamp(F32 n, F32 minRange, F32 maxRange)
	{
		if (n < minRange) return minRange;
		else if (n > maxRange) return maxRange;
		else return n;
	}

	inline F32 Saturate(F32 n)
	{
		if (n < 0.0f) return 0.0f;
		else if (n > 1.0f) return 1.0f;
		else return n;
	}

	bool SolveQuadratic(F32 a, F32 b, F32 c, F32& x0, F32& x1)
	{
		F32 delta = (b * b) - (4.0f * a * c);

		if (delta < 0.0f) return false;

		else if (delta == 0.0f)
		{
			x0 = -0.5f * (b / a);
			x1 = x0;

			return true;
		}

		F32 deltaSquared = sqrt(delta);
		F32 q = b > 0 ? -0.5f * (b + deltaSquared) : -0.5f * (b - deltaSquared);
		x0 = q / a;
		x1 = c / q;

		if (x1 < x0) std::swap<F32>(x0, x1);

		return true;
	}

	Vector3 operator+(const Vector3& v0, const Vector3& v1)
	{
		Vector3 v;

		v.x = v0.x + v1.x;
		v.y = v0.y + v1.y;
		v.z = v0.z + v1.z;

		return v;
	}

	Vector3 operator-(const Vector3& v0, const Vector3& v1)
	{
		Vector3 v;

		v.x = v0.x - v1.x;
		v.y = v0.y - v1.y;
		v.z = v0.z - v1.z;

		return v;
	}

	Vector3 operator*(const Vector3& v0, F32 f)
	{
		Vector3 v;

		v.x = v0.x * f;
		v.y = v0.y * f;
		v.z = v0.z * f;

		return v;
	}

	Vector3 operator/(const Vector3& v0, F32 f)
	{
		Vector3 v;

		v.x = v0.x / f;
		v.y = v0.y / f;
		v.z = v0.z / f;

		return v;
	}

	F32 Dot(const Vector3& v0, const Vector3& v1)
	{
		return ((v0.x * v1.x) + (v0.y * v1.y) + (v0.z * v1.z));
	}

	Vector3 Cross(const Vector3& v0, const Vector3& v1)
	{
		Vector3 v;
		v.x = (v0.y * v1.z) - (v0.z * v1.y);
		v.y = (v0.z * v1.x) - (v0.x * v1.z);
		v.z = (v0.x * v1.y) - (v0.y * v1.x);

		return v;
	}

	Vector3 Normalize(const Vector3& v0)
	{
		Vector3 v;

		F32 n = sqrtf((v0.x * v0.x) + (v0.y * v0.y) + (v0.z * v0.z));

		v.x = v0.x / n;
		v.y = v0.y / n;
		v.z = v0.z / n;

		return v;
	}

	F32 Length(const Vector3& v0)
	{
		return sqrtf((v0.x * v0.x) + (v0.y * v0.y) + (v0.z * v0.z));
	}

	Vector4 operator+(const Vector4& v0, const Vector4& v1)
	{
		Vector4 v;

		v.x = v0.x + v1.x;
		v.y = v0.y + v1.y;
		v.z = v0.z + v1.z;
		v.w = v0.w + v1.w;

		return v;
	}

	Vector4 operator-(const Vector4& v0, const Vector4& v1)
	{
		Vector4 v;

		v.x = v0.x - v1.x;
		v.y = v0.y - v1.y;
		v.z = v0.z - v1.z;
		v.w = v0.w - v1.w;

		return v;
	}

	Vector4 operator*(const Vector4& v0, F32 f)
	{
		Vector4 v;

		v.x = v0.x * f;
		v.y = v0.y * f;
		v.z = v0.z * f;
		v.w = v0.w * f;

		return v;
	}

	Vector4 operator/(const Vector4& v0, F32 f)
	{
		Vector4 v;

		v.x = v0.x / f;
		v.y = v0.y / f;
		v.z = v0.z / f;
		v.w = v0.w / f;

		return v;
	}

	F32 Dot(const Vector4& v0, const Vector4& v1)
	{
		return ((v0.x * v1.x) + (v0.y * v1.y) + (v0.z * v1.z) + (v0.w + v1.w));
	}

	Vector4 Normalize(const Vector4& v0)
	{
		Vector4 v;

		F32 n = sqrtf((v0.x * v0.x) + (v0.y * v0.y) + (v0.z * v0.z) + (v0.w * v0.w));

		v.x = v0.x / n;
		v.y = v0.y / n;
		v.z = v0.z / n;
		v.w = v0.z / n;

		return v;
	}

	F32 Length(const Vector4& v0)
	{
		return sqrtf((v0.x * v0.x) + (v0.y * v0.y) + (v0.z * v0.z) + (v0.w * v0.w));
	}

	Matrix3x3 operator*(const Matrix3x3& mat0, const Matrix3x3& mat1)
	{
		Matrix3x3 mat;

		mat.m00 = (mat0.m00 * mat1.m00) + (mat0.m01 * mat1.m10) + (mat0.m02 * mat1.m20);
		mat.m01 = (mat0.m00 * mat1.m01) + (mat0.m01 * mat1.m11) + (mat0.m02 * mat1.m21);
		mat.m02 = (mat0.m00 * mat1.m02) + (mat0.m01 * mat1.m12) + (mat0.m02 * mat1.m22);

		mat.m10 = (mat0.m10 * mat1.m00) + (mat0.m11 * mat1.m10) + (mat0.m12 * mat1.m20);
		mat.m11 = (mat0.m10 * mat1.m01) + (mat0.m11 * mat1.m11) + (mat0.m12 * mat1.m21);
		mat.m12 = (mat0.m10 * mat1.m02) + (mat0.m11 * mat1.m12) + (mat0.m12 * mat1.m22);

		mat.m20 = (mat0.m20 * mat1.m00) + (mat0.m21 * mat1.m10) + (mat0.m22 * mat1.m20);
		mat.m21 = (mat0.m20 * mat1.m01) + (mat0.m21 * mat1.m11) + (mat0.m22 * mat1.m21);
		mat.m22 = (mat0.m20 * mat1.m02) + (mat0.m21 * mat1.m12) + (mat0.m22 * mat1.m22);

		return mat;
	}

	Vector4 operator*(const Matrix3x3& mat0, const Vector3& v0)
	{
		Vector4 v;

		v.x = (mat0.m00 * v0.x) + (mat0.m01 * v0.y) + (mat0.m02 * v0.z);
		v.y = (mat0.m10 * v0.x) + (mat0.m11 * v0.y) + (mat0.m12 * v0.z);
		v.z = (mat0.m20 * v0.x) + (mat0.m21 * v0.y) + (mat0.m22 * v0.z);

		return v;
	}

	Matrix3x3 Transpose(const Matrix3x3& mat0)
	{
		Matrix3x3 mat;

		mat.m00 = mat0.m00; mat.m01 = mat0.m10; mat.m02 = mat0.m20;
		mat.m10 = mat0.m01; mat.m11 = mat0.m11; mat.m12 = mat0.m21;
		mat.m20 = mat0.m02; mat.m21 = mat0.m12; mat.m22 = mat0.m22;

		return mat;
	}


	Matrix4x4 operator*(const Matrix4x4& mat0, const Matrix4x4& mat1)
	{
		Matrix4x4 mat;

		mat.m00 = (mat0.m00 * mat1.m00) + (mat0.m01 * mat1.m10) + (mat0.m02 * mat1.m20) + (mat0.m03 * mat1.m30);
		mat.m01 = (mat0.m00 * mat1.m01) + (mat0.m01 * mat1.m11) + (mat0.m02 * mat1.m21) + (mat0.m03 * mat1.m31);
		mat.m02 = (mat0.m00 * mat1.m02) + (mat0.m01 * mat1.m12) + (mat0.m02 * mat1.m22) + (mat0.m03 * mat1.m32);
		mat.m03 = (mat0.m00 * mat1.m03) + (mat0.m01 * mat1.m13) + (mat0.m02 * mat1.m23) + (mat0.m03 * mat1.m33);

		mat.m10 = (mat0.m10 * mat1.m00) + (mat0.m11 * mat1.m10) + (mat0.m12 * mat1.m20) + (mat0.m13 * mat1.m30);
		mat.m11 = (mat0.m10 * mat1.m01) + (mat0.m11 * mat1.m11) + (mat0.m12 * mat1.m21) + (mat0.m13 * mat1.m31);
		mat.m12 = (mat0.m10 * mat1.m02) + (mat0.m11 * mat1.m12) + (mat0.m12 * mat1.m22) + (mat0.m13 * mat1.m32);
		mat.m13 = (mat0.m10 * mat1.m03) + (mat0.m11 * mat1.m13) + (mat0.m12 * mat1.m23) + (mat0.m13 * mat1.m33);

		mat.m20 = (mat0.m20 * mat1.m00) + (mat0.m21 * mat1.m10) + (mat0.m22 * mat1.m20) + (mat0.m23 * mat1.m30);
		mat.m21 = (mat0.m20 * mat1.m01) + (mat0.m21 * mat1.m11) + (mat0.m22 * mat1.m21) + (mat0.m23 * mat1.m31);
		mat.m22 = (mat0.m20 * mat1.m02) + (mat0.m21 * mat1.m12) + (mat0.m22 * mat1.m22) + (mat0.m23 * mat1.m32);
		mat.m23 = (mat0.m20 * mat1.m03) + (mat0.m21 * mat1.m13) + (mat0.m22 * mat1.m23) + (mat0.m23 * mat1.m33);

		mat.m30 = (mat0.m30 * mat1.m00) + (mat0.m31 * mat1.m10) + (mat0.m32 * mat1.m20) + (mat0.m33 * mat1.m30);
		mat.m31 = (mat0.m30 * mat1.m01) + (mat0.m31 * mat1.m11) + (mat0.m32 * mat1.m21) + (mat0.m33 * mat1.m31);
		mat.m32 = (mat0.m30 * mat1.m02) + (mat0.m31 * mat1.m12) + (mat0.m32 * mat1.m22) + (mat0.m33 * mat1.m32);
		mat.m33 = (mat0.m30 * mat1.m03) + (mat0.m31 * mat1.m13) + (mat0.m32 * mat1.m23) + (mat0.m33 * mat1.m33);

		return mat;
	}

	Vector4 operator*(const Matrix4x4& mat0, const Vector4& v0)
	{
		Vector4 v;

		v.x = (mat0.m00 * v0.x) + (mat0.m01 * v0.y) + (mat0.m02 * v0.z) + (mat0.m03 * v0.w);
		v.y = (mat0.m10 * v0.x) + (mat0.m11 * v0.y) + (mat0.m12 * v0.z) + (mat0.m13 * v0.w);
		v.z = (mat0.m20 * v0.x) + (mat0.m21 * v0.y) + (mat0.m22 * v0.z) + (mat0.m23 * v0.w);
		v.w = (mat0.m30 * v0.x) + (mat0.m31 * v0.y) + (mat0.m32 * v0.z) + (mat0.m33 * v0.w);

		return v;
	}

	Matrix4x4 Transpose(const Matrix4x4& mat0)
	{
		Matrix4x4 mat;

		mat.m00 = mat0.m00; mat.m01 = mat0.m10; mat.m02 = mat0.m20; mat.m03 = mat0.m30;
		mat.m10 = mat0.m01; mat.m11 = mat0.m11; mat.m12 = mat0.m21; mat.m13 = mat0.m31;
		mat.m20 = mat0.m02; mat.m21 = mat0.m12; mat.m22 = mat0.m22; mat.m23 = mat0.m32;
		mat.m30 = mat0.m03; mat.m31 = mat0.m13; mat.m32 = mat0.m23; mat.m33 = mat0.m33;

		return mat;
	}

	Matrix4x4 Translation(F32 Tx, F32 Ty, F32 Tz)
	{
		Matrix4x4 mat;

		mat.m00 = 1.0f; mat.m01 = 0.0f; mat.m02 = 0.0f; mat.m03 = Tx;
		mat.m10 = 0.0f; mat.m11 = 1.0f; mat.m12 = 0.0f; mat.m13 = Ty;
		mat.m20 = 0.0f; mat.m21 = 0.0f; mat.m22 = 1.0f; mat.m23 = Tz;
		mat.m30 = 0.0f; mat.m31 = 0.0f; mat.m32 = 0.0f; mat.m33 = 1.0f;

		return mat;
	}


	Matrix4x4 Scale(F32 Sx, F32 Sy, F32 Sz)
	{
		Matrix4x4 mat;

		mat.m00 = Sx;	mat.m01 = 0.0f; mat.m02 = 0.0f; mat.m03 = 0.0f;
		mat.m10 = 0.0f; mat.m11 = Sy;	mat.m12 = 0.0f; mat.m13 = 0.0f;
		mat.m20 = 0.0f; mat.m21 = 0.0f; mat.m22 = Sz;	mat.m23 = 0.0f;
		mat.m30 = 0.0f; mat.m31 = 0.0f; mat.m32 = 0.0f; mat.m33 = 1.0f;

		return mat;
	}

	Matrix4x4 RotationX(F32 deg)
	{
		F32 a = DegToRad(deg);
		F32 sina = sinf(a);
		F32 cosa = cosf(a);

		Matrix4x4 mat;

		mat.m00 = 1.0f;	mat.m01 = 0.0f;  mat.m02 = 0.0f; mat.m03 = 0.0f;
		mat.m10 = 0.0f; mat.m11 = cosa;	 mat.m12 = sina; mat.m13 = 0.0f;
		mat.m20 = 0.0f; mat.m21 = -sina; mat.m22 = cosa; mat.m23 = 0.0f;
		mat.m30 = 0.0f; mat.m31 = 0.0f;	 mat.m32 = 0.0f; mat.m33 = 1.0f;

		return mat;
	}

	Matrix4x4 RotationY(F32 deg)
	{
		F32 a = DegToRad(deg);
		F32 sina = sinf(a);
		F32 cosa = cosf(a);

		Matrix4x4 mat;

		mat.m00 = cosa;	mat.m01 = 0.0f; mat.m02 = -sina; mat.m03 = 0.0f;
		mat.m10 = 0.0f; mat.m11 = 1.0f;	mat.m12 = 0.0f;  mat.m13 = 0.0f;
		mat.m20 = sina; mat.m21 = 0.0f; mat.m22 = cosa;  mat.m23 = 0.0f;
		mat.m30 = 0.0f; mat.m31 = 0.0f;	mat.m32 = 0.0f;  mat.m33 = 1.0f;

		return mat;
	}

	Matrix4x4 RotationZ(F32 deg)
	{
		F32 a = DegToRad(deg);
		F32 sina = sinf(a);
		F32 cosa = cosf(a);

		Matrix4x4 mat;

		mat.m00 = cosa;	 mat.m01 = sina; mat.m02 = 0.0f; mat.m03 = 0.0f;
		mat.m10 = -sina; mat.m11 = cosa; mat.m12 = 0.0f; mat.m13 = 0.0f;
		mat.m20 = 0.0f;  mat.m21 = 0.0f; mat.m22 = 1.0f; mat.m23 = 0.0f;
		mat.m30 = 0.0f;  mat.m31 = 0.0f; mat.m32 = 0.0f; mat.m33 = 1.0f;

		return mat;
	}

	Matrix4x4 Rotation(const Vector3& v0, F32 deg)
	{
		F32 a = DegToRad(deg);

		F32 sina = sinf(a);
		F32 cosa = cosf(a);

		F32 iS = 1 - sina;
		F32 iC = 1 - cosa;

		F32 x2 = v0.x * v0.x;
		F32 y2 = v0.y * v0.y;
		F32 z2 = v0.z * v0.z;

		F32 xS = v0.x * sina;
		F32 yS = v0.y * sina;
		F32 zS = v0.z * sina;

		F32 iCxy = iC * v0.x * v0.y;
		F32 iCxz = iC * v0.x * v0.z;
		F32 iCyz = iC * v0.y * v0.z;

		Matrix4x4 mat;

		mat.m00 = x2 + ((1 - x2) * cosa); mat.m01 = iCxy - zS;				mat.m02 = iCxz + yS;			  mat.m03 = 0.0f;
		mat.m10 = iCxy + zS;			  mat.m11 = y2 + ((1 - y2) * cosa);	mat.m12 = iCyz - xS;			  mat.m13 = 0.0f;
		mat.m20 = iCxz - yS;			  mat.m21 = iCyz + xS;				mat.m22 = z2 + ((1 - z2) * cosa); mat.m23 = 0.0f;
		mat.m30 = 0.0f;					  mat.m31 = 0.0f;					mat.m32 = 0.0f;					  mat.m33 = 1.0f;

		return mat;
	}

	Matrix4x4 Perspective_GL(F32 fovDeg, F32 aspect, F32 zNear, F32 zFar)
	{
		F32 s = 1 / tanf(DegToRad(fovDeg) * 0.5f);

		Matrix4x4 mat;

		mat.m00 = s;	mat.m01 = 0.0f;		  mat.m02 = 0.0f;									 mat.m03 = 0.0f;
		mat.m10 = 0.0f; mat.m11 = s * aspect; mat.m12 = 0.0f;									 mat.m13 = 0.0f;
		mat.m20 = 0.0f; mat.m21 = 0.0f;		  mat.m22 = -1.0f * (zFar + zNear) / (zFar - zNear); mat.m23 = (-2.0f * zFar * zNear) / (zFar - zNear);
		mat.m30 = 0.0f;	mat.m31 = 0.0f;		  mat.m32 = -1.0f;									 mat.m33 = 0.0f;

		return mat;
	}

	Matrix4x4 PerspectiveFovMatrix(F32 FovYDeg, F32 AspectRatio, F32 ZNear, F32 ZFar)
	{
		F32 ScaleY = 1 / tanf(DegToRad(FovYDeg) * 0.5f);
		F32 ScaleX = ScaleY / AspectRatio;
		Matrix4x4 mat;

		mat.m00 = ScaleX;	  mat.m01 = 0.0f;	mat.m02 = 0.0f;								mat.m03 = 0.0f;
		mat.m10 = 0.0f;       mat.m11 = ScaleY;	mat.m12 = 0.0f;								mat.m13 = 0.0f;
		mat.m20 = 0.0f;       mat.m21 = 0.0f;	mat.m22 = ZFar / (ZNear - ZFar);			mat.m23 = -1.0f;
		mat.m30 = 0.0f;	      mat.m31 = 0.0f;	mat.m32 = (ZNear * ZFar) / (ZNear - ZFar);	mat.m33 = 0.0f;

		return mat;
	}

	Matrix4x4 PerspectiveFovMatrixLH(F32 FovYDeg, F32 AspectRatio, F32 ZNear, F32 ZFar)
	{
		F32 ScaleY = 1 / tanf(DegToRad(FovYDeg) * 0.5f);
		F32 ScaleX = ScaleY / AspectRatio;
		Matrix4x4 mat;

		mat.m00 = ScaleX;	  mat.m01 = 0.0f;	mat.m02 = 0.0f;								mat.m03 = 0.0f;
		mat.m10 = 0.0f;       mat.m11 = ScaleY;	mat.m12 = 0.0f;								mat.m13 = 0.0f;
		mat.m20 = 0.0f;       mat.m21 = 0.0f;	mat.m22 = ZFar / (ZFar - ZNear);			mat.m23 = 1.0f;
		mat.m30 = 0.0f;	      mat.m31 = 0.0f;	mat.m32 = (ZNear * ZFar) / (ZNear - ZFar);	mat.m33 = 0.0f;

		return mat;
	}

	Matrix4x4 Perspective(F32 l, F32 r, F32 b, F32 t, F32 zNear, F32 zFar)
	{
		Matrix4x4 mat;

		mat.m00 = (2.0f * zNear) / (r - l); mat.m01 = 0.0f;					    mat.m02 = (r + l) / (r - l);					   mat.m03 = 0.0f;
		mat.m10 = 0.0f;						mat.m11 = (2.0f * zNear) / (t - b); mat.m12 = (t + b) / (t - b);					   mat.m13 = 0.0f;
		mat.m20 = 0.0f;						mat.m21 = 0.0f;					    mat.m22 = -1.0f * (zFar + zNear) / (zFar - zNear); mat.m23 = (-2.0f * zFar * zNear) / (zFar - zNear);
		mat.m30 = 0.0f;						mat.m31 = 0.0f;		                mat.m32 = -1.0f;								   mat.m33 = 0.0f;

		return mat;
	}


	Matrix4x4 Ortographic(F32 l, F32 r, F32 b, F32 t, F32 zNear, F32 zFar)
	{
		Matrix4x4 mat;

		mat.m00 = 2.0f / (r - l); mat.m01 = 0.0f;		    mat.m02 = 0.0f;					  mat.m03 = -1.0f * (r + l) / (r - l);
		mat.m10 = 0.0f;			  mat.m11 = 2.0f / (t - b); mat.m12 = 0.0f;					  mat.m13 = -1.0f * (t + b) / (t - b);
		mat.m20 = 0.0f;			  mat.m21 = 0.0f;			mat.m22 = -2.0f / (zFar - zNear); mat.m23 = -1.0f * (zFar + zNear) / (zFar - zNear);
		mat.m30 = 0.0f;			  mat.m31 = 0.0f;		    mat.m32 = 0.0f;					  mat.m33 = 1.0f;

		return mat;
	}

	Matrix4x4 LookAtMatrix(const Vector3& Eye, const Vector3 &Target, const Vector3& Up)
	{
		Matrix4x4 mat;

		Vector3 AxisZ = Normalize(Eye - Target);
		Vector3 AxisX = Normalize(Cross(Up, AxisZ));
		Vector3 AxisY = Cross(AxisZ, AxisX);

		mat.m00 = AxisX.x;			mat.m01 = AxisY.x;		    mat.m02 = AxisZ.x;			mat.m03 = 0.0f;
		mat.m10 = AxisX.y;			mat.m11 = AxisY.y;			mat.m12 = AxisZ.y;			mat.m13 = 0.0f;
		mat.m20 = AxisX.z;			mat.m21 = AxisY.z;			mat.m22 = AxisZ.z;			mat.m23 = 0.0f;
		mat.m30 = -Dot(AxisX, Eye);	mat.m31 = -Dot(AxisY, Eye);	mat.m32 = -Dot(AxisZ, Eye);	mat.m33 = 1.0f;

		return mat;
	}

	Matrix4x4 LookAtMatrixLH(const Vector3& Eye, const Vector3 &Target, const Vector3& Up)
	{
		Matrix4x4 mat;

		Vector3 AxisZ = Normalize(Target - Eye);
		Vector3 AxisX = Normalize(Cross(Up, AxisZ));
		Vector3 AxisY = Cross(AxisZ, AxisX);

		mat.m00 = AxisX.x;			mat.m01 = AxisY.x;		    mat.m02 = AxisZ.x;			mat.m03 = 0.0f;
		mat.m10 = AxisX.y;			mat.m11 = AxisY.y;			mat.m12 = AxisZ.y;			mat.m13 = 0.0f;
		mat.m20 = AxisX.z;			mat.m21 = AxisY.z;			mat.m22 = AxisZ.z;			mat.m23 = 0.0f;
		mat.m30 = -Dot(AxisX, Eye);	mat.m31 = -Dot(AxisY, Eye);	mat.m32 = -Dot(AxisZ, Eye);	mat.m33 = 1.0f;

		return mat;
	}

}

//#include "DirectXMath.h"