#ifndef _H_2D_GEOMETRY_
#define _H_2D_GEOMETRY_
#include "vectors.h"

typedef vec2 Point2D;

typedef struct Line2D
{
	Point2D start;
	Point2D end;

	inline Line2D(){}
	inline Line2D(const Point2D& s, const Point2D& e)
		:start(s), end(e){}
};

float Length(const Line2D& line);
float LengthSq(const Line2D& line);

typedef struct Circle
{
	Point2D position;
	float radius;

	inline Circle() : radius(1.0f) {}
	inline Circle(const Point2D& p, float r): position(p), radius(r) {}
} Circle;

typedef struct Rectangle2D 
{
	Point2D origin;
	vec2 size;

	inline Rectangle2D() : size(1.0f, 1.0f) {}
	inline Rectangle2D(const Point2D& o, const vec2& s) : origin(o), size(s) {}
} Rectangle2D;

typedef struct OrientedRectangle
{
	Point2D position;
	vec2 halfExtents;
	float rotation;

	inline OrientedRectangle() : halfExtents(1.0f, 1.0f), rotation(0.0f) {}
	inline OrientedRectangle(const Point2D& p, const vec2& 2) : position(p), halfExtents(e), rotation(0.0f) {} 
	inline OrientedRectangle(const Point2D& pos, const vec2& ext, float rot) : position(pos), halfExtents(ext), rotation(rot) {}
} OrientedRectangle;

typedef struct Interval2D 
{
	float min;
	float max;
} Interval2D;

Interval2D GetInterval(const Rectangle2D& rect, const vec2& axis)
{
	Interval2D result;
	vec2 min = GetMin(rect);
	vec2 max = GetMax(rect);

	vec2 verts[] {
		vec2(min.x, min.y), vec2(min.x, max.y),
		vec2(max.x, max.y), vec2(max.x, min.y)
	};

	result.min = result.max = Dot(axis, verts[0]);
	for (int i = 1; i < 4; i++)
	{
		float projection = Dot(axis, verts[i]);
		if (projection < result.min)
		{
			result.min = projection;
		}
		if (projection > result.max)
		{
			result.max = projection;
		}
	}
	return result;
}
Interval2D GetInterval(const Rectangle2D& rect, const vec2& axis);
Interval2D GetInterval(const OrientedRectangle& rect, const vec2& axis)
{
	Rectangle2D r = Rectangle2D(
		Point2D(rect.position - rect.halfExtents), 
		rect.halfExtents * 2.0f
	);

	vec2 min = GetMin(r);
	vec2 max = GetMax(r);
	vec2 verts[] = {
		min, max,
		vec2(min.x, max.y), vec2(max.x, min.y)
	};
	
	float t = DEG2RAD(rect.rotation);
	float zRot[] = {
		cosf(t), sinf(t),
		-sinf(t), cosf(t)
	};

	for (int i = 0; i < 4; i++)
	{
		vec2 r = verts[i] - rect.position;
		Multiply(r.asArray, vec2(r.x, r.y).asArray, 1, 2, zRot, 2, 2);
		verts[i] = r + rect.position;
	}

	Interval2D res;
	res.min = res.max = Dot(axis, verts[0]);
	for (int i = 1; i < 4; i++)
	{
		float proj = Dot(axis, verts[i]);
		res.min = (proj < res.min) ? proj : res.min;
		res.max = (proj > res.max) ? proj : res.max;
	}
	return res;
}



vec2 GetMin(const Rectangle2D& rect);
vec2 GetMax(const Rectangle2D& rect);
Rectangle2D FromMinMax(const vec2& min, const vec2& max);
Rectangle2D FromMinMax(const vec2& min, const vec2& max)
{
	return Rectangle2D(min, max - min);
}

bool PointInLine(const Point2D& point, const Line2D line);
bool PointInCircle(const Point2D& point, const Circle& c);
bool PointInRectangle(const Point2D& point, const Rectangle2D& rectangle);
bool PointInOrientedRectangle(const Point2D& point, const OrientedRectangle& rectangle);

bool LineCircle(const Line2D& line, const Circle& circle);
bool LineRectanlge(const Line2D& l, const Rectangle2D& r);
bool LineOrientedRectangle(const Line2D& line, const OrientedRectangle &rectangle);

bool CircleCircle(const Circle& c1, const Circle& c2);
bool CircleRectangle(const Circle& circle, const Rectangle2D& rectangle);
bool CircleOrientedRectangle(const Circle& circle, const OrientedRectangle rectangle);

bool RectangleRectangle(const Rectangle2D& rect1, const Rectangle2D& rect2);
bool OverlapOnAxis(const Rectangle2D rect1, const Rectangle2D rect2);
bool RectangleRectangleSAT(const Rectangle2D& rect1, const Rectangle2D& rect2);
bool OverlapOnAxis(const Rectangle2D& rect1, const OrientedRectangle& rect2, const vec2& axis);

bool RectangleOrientedRectangle(const Rectangle2D& rect1, const OrientedRectangle& rect2);

#define PointLine(point, line) \
	PointOnLine(point, line)
#define LinePoint(line, point) \
	PointOnLine(point, line)
#define CircleLine(circle, line) \
	LineCircle(line, circle)
#define RectangleLine(rectangle, line) \
	LineRectangle(line, rectangle)
#define OrientedRetangleLine(rectangle, line) \
	LineOrientedRectangle(line, rectangle)
#define RectangleCircle(rectangle, circle) \
	CircleRectangle(circle, rectangle)
#define OrientedRectangleCircle(rectangle, circle) \
	CircleOrientedRectangle(circle, rectangle)
#define OrientedRectangleRectangle(oriented, regular) \
	RectangleOrientedRectangle(regular, oriented);


#endif