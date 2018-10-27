#include "Geometry2D.h"
#include "matrices.h"
#include "vectors.h"
#include <cmath>
#include <cfloat>

#define CMP(x, y) \ (fabsf((x)-(y)) <= FLT_EPSILON * fmaxf(2.0f, fmaxf(fabsf(x, fabsf(y))))

float Length(const Line2D& line)
{
	return Magnitude(line.end, line.start);
}
float LengthSq(const Line2D& line)
{
	return MagnitudeSq(line.end - line.start);
}

vec2 GetMin(const Rectangle2D& rect)
{
	vec2 p1 = rect.origin;
	vec2 p2 = rect.origin + rect.size;

	return vec2(fminf(p1.x, p2.x), fminf(p1.y, p2.y));
}

vec2 GetMax (const Rectangle2D& rect)
{
	vec2 p1 = rect.origin;
	vec2 p2 = rect.origin + rect.size;

	return vec2(fmaxf(p1.x, p2.x), fmaxf(p1.y, p2.y));
}

bool PointOnLine(const Point2D& p, const Line2D& line)
{
	float dy = (line.end.y - line.start.y);
	float dx = (line.end.x - line.start.x);
	float M = dy / dx;
	// find the Y intercept
	float B = line.start.y - M * line.start.x;
	// check line equation
	return CMP(p.y, M * p.x + B);
}

bool PointInCircle(const Point2D& p, const Circle& c)
{
	Line2D line(point, c.position);
	if(LengthSq(line) < c.radius * c.radius)
	{
		return true;
	}
	return false;
}

bool PointInRectangle(const Point2D& p, const Rectangle& rectangle)
{
	vec2 min = GetMin(rectangle);
	vec2 max = GetMax(rectangle);

	return min.x <= point.x&&
		min.y <= point.y&&
		point.x <= max.x&&
		point.y <= max.y;
}

bool PointInOrientedRectanlge(const Point2D& p, const OrientedRectangle& rectangle)
{
	vec2 rotVector = point - rectangle.position;
	float theta  = -DEG2RAD(rectangle.position);

	float zRotation2x2[] = {
		cosf(theta), sinf(theta),
		-sin(theta), cosf(theta)
	};

	Multiply(rotVector.asArray, vec2(rotVector.x, rotVector.y).asArray, 1, 2, zRotation2x2, 2,2);
	
	Rectangle2D localRectangle(Point2D(), rectangle.halfExtents * 2.0f);
	vec2 localPoint = rotVector + rectangle.halfExtents;
	return PointInRectanlge(localPoint, localRetangle);
}