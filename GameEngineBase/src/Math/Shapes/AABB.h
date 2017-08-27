#pragma once

#include "ge.h"
#include "Common.h"	
#include "Math/Vector2f.h"
#include "Math/Vector3f.h"

namespace gebase { namespace math
{
	struct Rectangle;

	struct GE_API AABB
	{
		Vector3f min;
		Vector3f max;

		AABB();
		AABB(const Rectangle& rectangle);
		AABB(const Vector2f& min, const Vector2f& max);
		AABB(const Vector3f& min, const Vector3f& max);
		AABB(float x, float y, float width, float height);
		AABB(float x, float y, float z, float width, float height, float depth);

		bool Intersects(const AABB& other);
		bool Contains(const Vector2f& point);
		bool Contains(const Vector3f& point);

		inline Vector3f Center() const { return (max + min) * 0.5f; }
		inline Vector3f getSize() const { return Vector3f(abs(max.x - min.x), abs(max.y - min.y), abs(max.z - min.z)); }

		inline bool operator==(const AABB& other) const { return min == other.min && max == other.max; }
		inline bool operator!=(const AABB& other) const { return !(*this == other); }
		inline bool operator<(const AABB& other) const { return max < other.min; }
		inline bool operator>(const AABB& other) const { return min > other.max; }
		inline bool operator<=(const AABB& other) const { return (*this < other) || (*this == other); }
		inline bool operator>=(const AABB& other) const { return (*this > other) || (*this == other); }

		friend std::ostream& operator<<(std::ostream& stream, const AABB& aabb);
	};
} }