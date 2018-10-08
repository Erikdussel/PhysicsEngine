#pragma once
#include "vectors.h"

#ifndef _H_MATH_MATRICES_
#define _H_MATH_MATRICES_

typedef struct mat2 {
	union {
		struct {
			float _11, _12,
				_21, _22;
		};
		float asArray[4];
	};
	inline float* operator[](int i)
	{
		return &(asArray[i * 2]);
	};
	inline mat2() 
	{
		_11 = _22 = 1.0f;
		_12 = _21 = 0.0f;
	};
} mat2;

typedef struct mat3 {
	union {
		struct {
			float _11, _12, _13,
				_21, _22, _23,
				_31, _32, _33;
		};
		float asArray[9];
	};
	inline float* operator[](int i)
	{
		return &(asArray[i * 3]);
	};
	inline mat3()
	{
		_11 = _22 = _33 = 1.0f;
		_12 = _13 = _21 = 0.0f;
		_23 = _31 = _32 = 0.0f;
	};
} mat3;

typedef struct mat4 {
	union {
		struct {
			float _11, _12, _13, _14,
				_21, _22, _23, _24,
				_31, _32, _33, _34,
				_41, _42, _43, _44;
		};
		float asArray[16];
	};
	inline float* operator[](int i)
	{
		return &(asArray[i * 4]);
	};
	inline mat4()
	{
		_11 = _22 = _33 = _44 = 1.0f;
		_12 = _13 = _13 = _21 = 0.0f;
		_23 = _24 = _31 = _32 = 0.0f;
		_34 = _41 = _42 = _43 = 0.0f;
	};
} mat4;


// transpose declarations;
void Transpose(const float *srcMat, float *dstMat, int srcRows, int srcCols);
mat2 Transpose(const mat2& matrix);
mat3 Transpose(const mat3& matrix);
mat4 Transpose(const mat4& matrix);

// scalar mutliplcation declarations
mat2 operator*(const mat2& matrix, float scalar);
mat3 operator*(const mat3& matrix, float scalar);
mat4 operator*(const mat4& matrix, float scalar);

// matrix-to-matrix multiplication declarations
bool Multiply(float* out, const float* matA, int aRows, int aCols, const float* matB, int bRows, int bCols);
mat2 operator*(const mat2& matA, const mat2& matB);
mat3 operator*(const mat3& matA, const mat3& matB);
mat4 operator*(const mat4& matA, const mat4& matB);

#endif