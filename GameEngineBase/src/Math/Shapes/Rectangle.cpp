#include "Pch.h"
#include "Rectangle.h"
#include "AABB.h"

namespace gebase { namespace math {
	Rectangle::Rectangle() : position(Vector2f()), size(Vector2f())
	{

	}

	Rectangle::Rectangle(const AABB& aabb) : position(Vector2f(aabb.min)), size(Vector2f(aabb.getSize()))
	{

	}

	Rectangle::Rectangle(const Vector2f& pos, const Vector2f& s) : position(pos), size(s)
	{

	}

	Rectangle::Rectangle(float x, float y, float width, float height) : position(Vector2f(x, y)), size(Vector2f(width, height))
	{

	}

	bool Rectangle::Intersects(const Rectangle& other) const
	{
		return (size > other.position && position < other.size) || (position > other.size && size < other.position);
	}

	bool Rectangle::Contains(const Vector2f& point) const
	{
		return point > getMinimumBound() && point < getMaximumBound();
	}

	bool Rectangle::Contains(const Vector3f& point) const
	{
		return Contains(Vector2f(point));
	}
} }