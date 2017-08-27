#include "ge.h"
#include "AABB.h"
#include "Rectangle.h"

namespace gebase { namespace math {

	AABB::AABB() : min(Vector3f()), max(Vector3f())
	{

	}

	AABB::AABB(const Rectangle& rectangle) : min(Vector3f(rectangle.getMinimumBound())), max(Vector3f(rectangle.getMaximumBound()))
	{

	}

	AABB::AABB(const Vector2f& min, const Vector2f& max) : min(Vector3f(min)), max(Vector3f(max))
	{

	}

	AABB::AABB(const Vector3f& min, const Vector3f& max) : min(min), max(max)
	{

	}

	AABB::AABB(float x, float y, float width, float height) : min(Vector3f(x, y, 0.0f)), max(Vector3f(x+width, y+height, 0.0f))
	{

	}

	AABB::AABB(float x, float y, float z, float width, float height, float depth) : min(Vector3f(x, y, z)), max(Vector3f(x+width, y+height, z+depth))
	{

	}

	bool AABB::Intersects(const AABB& other)
	{
		return (max > other.max && min < other.max) || (min > other.max && max < other.min);
	}

	bool AABB::Contains(const Vector2f& point)
	{
		return Contains(Vector3f(point));
	}

	bool AABB::Contains(const Vector3f& point)
	{
		return point > min && point < max;
	}

} }