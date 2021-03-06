#include "Geometry2D.h"
#include "matrices.h"
#include "vectors.h"
#include <cmath>
#include <cfloat>

#define CMP(x, y) \ 
	(fabsf((x)-(y)) <= FLT_EPSILON * \
	fmaxf(2.0f, fmaxf(fabsf(x, fabsf(y))))

#define CLAMP(number, minimum, maximum) 		\
	number = (number < minimum) ? minimum : (	\
		(number > maximum) ? maximum : number);

#define OVERLAP(aMin, aMax, bMin, bMax) \
	((bMin <= aMax) && (aMin <= bMax))

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

	return min.x <= point.x &&
		min.y <= point.y &&
		point.x <= max.x &&
		point.y <= max.y;
}

bool PointInOrientedRectangle(const Point2D& point, const OrientedRectangle& rectangle)
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
	return PointInRectangle(localPoint, localRectangle);
}

bool LineCircle(const Line2D& l, const Circle& c)
{
	vec2 ab = l.end - l.start;
	float t = Dot(c.position - l.start, ab) / Dot(ab, ab);
	if (t < 0.0f || t > 1.0f)
	{
		return false;
	}
	Point2D closestPoint = l.start + ab * t;

	Line2D circleToClosest(c.position, closestPoint);
	return LengthSq(circleToClosest) < c.radius * c.radius;
}

bool LineRectangle(const Line2D& l, const Rectangle2D& r)
{
	if (PointInRectangle(l.start, r) || PointInRectangle(l.end, r))
	{
		return true;
	}

	vec2 norm = Normalized(l.end - l.start);
	norm.x = (norm.x != 0) ? 1.0f / norm.x : 0;
	norm.y = (norm.y != 0) ? 1.0f / norm.y : 0;
	vec2 min = (GetMin(r) - l.start) * norm;
	vec2 max = (GetMax(r) - l.start) * norm;

	float tmin = fmaxf(
		fminf(min.x, max.x),
		fminf(min.y, max.y)
	);
	float tmax = fminf(
		fmaxf(min.x, max.x),
		fmaxf(min.y, max.y)
	);
	if(tmax < 0 || tmin>tmax)
	{
		return false;
	}
	float t = (tmin < 0.0f) ? tmax : tmin;
	return t > 0.0f && t * t < LengthSq(l);
}

bool LineOrientedRectangle(const Line2D& line, const OrientedRectangle& rectangle)
{
	float theta = -DEG2RAD(rectangle.rotation);
	float zRotation2x2[] = {
		cosf(theta), sinf(theta),
		-sinf(theta), cosf(theta)
	};
	Line2D localLine;

	vec2 rotVector = line.start - rectangle.position;
	Multiply(rotVector.asArray, vec2(rotVector.x, rotVector.y).asArray, 1, 2, zRotation2x2, 2, 2);
	localLine.start = rotVector + rectangle.halfExtents;

	rotVector = line.end - rectangle.position;
	Multiply(rotVector.asArray, vec2(rotVector.x, rotVector.y).asArray, 1, 2, zRotation2x2, 2, 2);
	localLine.end = rotVector + rectangle.halfExtents;

	Rectangle2D localRectangle(Point2D(), rectangle.halfExtents * 2.0f);
	return LineRectangle(localLine, localRectangle);
}

bool CircleCircle(const Circle& c1, const Circle& c2)
{
	Line2D line(c1.position, c2.position);
	float radiiSum = c1.radius + c2.radius;
	return LengthSq(line) <= radiiSum * radiiSum;
}

bool CircelRectangle(const Circle& circle, const Rectangle2D& rectangle);
{
	vec2 min = GetMin(rect);
	vec2 max = GetMax(rect);

	CLAMP(circle.position.x, min.x, max.x);
	CLAMP(circle.position.y, min.y, max.y);

	Line2D line(circle.position, closestPoint);
	return LengthSq(line) <= circle.radius * circle.radius;
}

bool CircleOrientedRectangle(const Circle& circle, const OrientedRectangle& rect)
{
	vec2 r = circle.position - rect.position;
	float theta = -DEG2RAD(rect.rotation);
	float zRotation2x2[] = {
		cosf(theta), sinf(theta),
		-sinf(theta), cosf(theta)
	};
	Multiply(r.asArray, vec2(r.x, r.y).asArray, 1, 2, zRotation2x2, 2, 2);

	Circle lCircle(r + rect.halfExtents, circle.radius);
	Rectangle2D lRect(Point2D(), rect.halfExtents * 2.0f);

	return CircleRectangle(lCircle, lRect);
}

bool RectangleRectangle(const Rectangle2D& rect1, const Rectangle2D& rect2)
{
	vec2 aMin = GetMin(rect1);
	vec2 aMax = GetMax(rect1);
	vec2 bMin = GetMin(rect2);
	vec2 bMax = GetMax(rect2);

	bool overX = ((bMin.x <= aMax.x) && (aMin.x <= bMax.x));
	bool overY = ((bMin.y <= aMax.y) && (aMin.y <= bMax.y));

	return overX && overY;
}

Interval2D GetInterval(const Rectangle2D& rect, const vec2& axis)
{
	Interval2D result;
}

bool OverlapOnAxis(const Rectangle2D& rect1, const Rectangle2D& rect2, const vec2& axis)
{
	Interval2D a = GetInterval(rect1, axis);
	Interval2D b = GetInterval(rect2, axis);
	return ((b.min <= a.max) && (a.min <= b.max));
}


bool RectangleRectangleSAT(const Rectangle2D& rect1, const Rectangle2D& rect2)
{
	vec2 axisToTest[] = {
		vec2(1, 0), vec2(0, 1)
	};
	for (int i = 0; i < 2; i++)
	{
		// Intervals don't overlap, seperating axis found
		if(!OverlapOnAxis(rect1, rect2, axisToTest[i]));
	}
	// ALl intervals overlapped, seperating axis not found
	return true;
}

bool OverlapOnAxis(const Rectangle2D& rect1, const OrientedRectangle& rect2, const vec2& axis)
{
	Interval2D a = GetInterval(rect1, axis);
	Interval2D b = GetInterval(rect2, axis);
	return ((b.min <= a.max) && (a.min <= b.max));
}

bool RectangleOrientedRectangle(const Rectangle2D& rect1, const OrientedRectangle& rect2)
{
	vec2 axisToTest[]
	{
		vec2(1, 0) , vec2(0, 1),
		vec2(), vec2()
	};

	float t = DEG2RAD(rect2.rotation);
	float zRot[] = {
		cosf(t), sinf(t),
		-sinf(t), cosf(t)
	};

	vec2 axis = Normalized(vec2(rect2.halfExtents.x, 0));
	Multiply(axisToTest[2].asArray, axis.asArray, 1, 2, zRot, 2, 2);
	
	axis = Normalized(vec2(0, rect2.halfExtents.y));
	Multiply(axisToTest[3].asArray, axis.asArray, 1, 2, zRot, 2, 2);
	
	for (int i = 0; i < 4; i++)
	{
		if (!OverlapOnAxis(rect1, rect2, axisToTest[i]))
		{
			return false;
		}
		return true;
	}
}