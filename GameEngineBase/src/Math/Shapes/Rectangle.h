#pragma once

#include "ge.h"
#include "Common.h"

#include "Math/Vector2f.h"
#include "Math/Vector3f.h"

namespace gebase { namespace math {

	struct AABB;

	struct GE_API Rectangle
	{
		union
		{
			Vector2f position;

			struct
			{
				float x;
				float y;
			};
		};

		union
		{
			Vector2f size;

			struct
			{
				float width;
				float height;
			};
		};

		Rectangle();
		Rectangle(const AABB& aabb);
		Rectangle(const Vector2f& position, const Vector2f& size);
		Rectangle(float x, float y, float width, float height);

		bool Intersects(const Rectangle& other) const;
		bool Contains(const Vector2f& point) const;
		bool Contains(const Vector3f& point) const;

		inline Vector2f getMinimumBound() const { return position - size; }
		inline Vector2f getMaximumBound() const { return position + size; }

		inline void operator=(const Rectangle& other) { this->position = other.position; this->size = other.size; }
		inline bool operator==(const Rectangle& other) const { return position == other.position && size == other.size; }
		inline bool operator!=(const Rectangle& other) const { return !(*this == other); }
		inline bool operator<(const Rectangle& other) const { return size < other.size; }
		inline bool operator>(const Rectangle& other) const { return size > other.size; }
		inline bool operator<=(const Rectangle& other) const { return (*this < other) || (*this == other); }
		inline bool operator>=(const Rectangle& other) const { return (*this > other) || (*this == other); }

		friend std::ostream& operator<<(std::ostream& stream, const Rectangle& rectangle);
	};

} }