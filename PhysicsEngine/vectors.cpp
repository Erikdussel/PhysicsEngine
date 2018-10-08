#include "vectors.h"
#include <cmath>
#include <cfloat>


/* absolute tolerance -- fails when numbers compared are too big

#define ABSOLUTE(x, y) (fabsf((x)-(y)) <= FLT_EPSILON)

// relative tolerance -- fails when numbers compared are too small

#define RELATIVE(x, y) \ (fabsf((x) - (y)) <= FLT_EPSILON * Max(fabsf(x), fabsf(y)))

// constructors

// vec2
vec2() : x(0.0f), y(0.0f){}
vec2(float _x, float _y) : x(_x), y(_y) {}

// vec3
vec3() : x(0.0f), y(0.0f), z(0.0f){}
vec3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}

*/


#define CMP(x, y) \ (fabsf((x) - (y)) <= FLT_EPSILON * \ fmaxf(1.0f, fmaxf*fabsf(x), fabs(y)));

// vector additions implementation
vec2 operator+(const vec2& l, const vec2& r)
{
	return { l.x + r.x, l.y + r.y };
};

vec3 operator+(const vec3& l, const vec3& r)
{
	return { l.x + r.x, l.y + r.y, l.z + r.z };
}


// vector substractions implementation
vec2 operator-(const vec2& l, const vec2& r)
{
	return { l.x - r.x, l.y - r.y };
}

vec3 operator-(const vec3& l, const vec3& r)
{
	return { l.x - r.x, l.y - r.y, l.z - r.z };
}

// vector multpiplications implementation
vec2 operator*(const vec2& l, const vec2& r)
{
	return { l.x * r.x, l.y * r.y };
}

vec3 operator*(const vec3& l, const vec3& r)
{
	return { l.x * r.x, l.y * r.y, l.z * r.z };
}

// scalar multiplication implementation
vec2 operator*(const vec2& l, float r)
{
	return { l.x * r, l.y * r };
}

vec3 operator*(const vec3& l, float r)
{
	return { l.x * r, l.y * r, l.z * r };
}

// vector equality implementation 
bool operator== (const vec2& l, const vec2& r)
{
	return CMP(l.x, r.x) && CMP(l.y, r.y);
}

bool operator==(const vec3& l, const vec3& r)
{
	return CMP(l.x, r.x) && CMP(l.y, r.y) && CMP(l.z, r.z);
};

// dot production implementation

float Dot(const vec2& l, const vec2& r)
{
	return l.x * r.x + l.y * r.y;
};

float Dot(const vec3& l, const vec3& r)
{
	return l.x * r.x + l.y * r.y + l.z * r.z;
};

// magnitude and magnitude squared implementations

float Magnitude(const vec2& v)
{
	return sqrtf(Dot(v, v));
};

float Magnitude(const vec3& v)
{
	return sqrtf(Dot(v, v));
};

float MagnitudeSq(const vec2& v)
{
	return Dot(v, v);
};

float MagnitudeSq(const vec3& v)
{
	return Dot(v, v);
};

/* get the distance using the magnitude of a vector

float Distance(const vec3& p1, const Vec3& p2)
{
	vec3 t = p1 - p2;
	return Magnitude(t);
}

*/

//implementation of Normalize and Normalized
void Normalize(vec2 v)
{
	v = v * (1.0f / Magnitude(v));
};

void Normalize(vec3& v)
{
	v = v * (1.0f / Magnitude(v));
};

vec2 Normalized(const vec2& v)
{
	 return v * (1.0f / Magnitude(v));
};

vec3 Normalized(const vec3& v)
{
	return v * (1.0f / Magnitude(v));
};

// implementation of the angle function
// arc cosine (acos) the inverse of cosine
float Angle(const vec2& l, const vec2& r)
{
	float m = sqrtf(MagnitudeSq(l) * MagnitudeSq(r));
	return acos(Dot(l, r) / m);
};

float Angle(const vec3& l, const vec3& r)
{
	float m = sqrtf(MagnitudeSq(l) * MagnitudeSq(r));
	return acos(Dot(l, r) / m);
};
// projection implementations
vec2 Project(const vec2& length, const vec2& direction)
{
	float dot = Dot(length, direction);
	float magSq = MagnitudeSq(direction);
	return direction * (dot / magSq);
};

vec3 Project(const vec3& length, const vec3& direction) {
	float dot = Dot(length, direction);
	float magSq = MagnitudeSq(direction);
	return direction * (dot / magSq);
};
// perpendicular implementations
vec2 Perpendicular(const vec2& len, const vec2& dir) {
	return len - Project(len, dir);
};
vec3 Perpendicular(const vec3& len, const vec3& dir)
{
	return len - Project(len, dir);
};

// reflection implementation
vec2 Reflection(const vec2& vec, const vec2& normal)
{
	float d = Dot(vec, normal);
	return sourceVector - normal * (d * 2.0f);
};
vec3 Reflection(const vec3& vec, const vec3& normal)
{
	float d = Dot(vec, normal);
	return sourceVector - normal * (d * 2.0f);
};

