#pragma once
#include <type_traits>
#include "Vec3.hpp"
#include <algorithm>

template <class T>
struct Box
{
	static_assert(std::is_arithmetic<T>::value, "T must be numeric");
	Box();
	explicit Box(const Math::Vec3<T> points[]);

	Box(const Box& other)
		: min(other.min),
		  max(other.max)
	{
	}

	Box(Box&& other) noexcept
		: min(std::move(other.min)),
		  max(std::move(other.max))
	{
	}

	Box& operator=(const Box& other)
	{
		if (this == &other)
			return *this;
		min = other.min;
		max = other.max;
		return *this;
	}

	Box& operator=(Box&& other) noexcept
	{
		if (this == &other)
			return *this;
		min = std::move(other.min);
		max = std::move(other.max);
		return *this;
	}

	void Boundaries(const Math::Vec3<T> points[]);
	
	Math::Vec3<T> min;
	Math::Vec3<T> max;
};

template <class T>
Box<T>::Box()
{
}

template <class T>
Box<T>::Box(const Math::Vec3<T> points[])
{
	Boundaries(points);
}

template <class T>
void Box<T>::Boundaries(const Math::Vec3<T> points[])
{
	for (unsigned i = 0; i < sizeof(points); ++i)
	{
		min.x = std::min(min.x, points[i].x);
		min.y = std::min(min.y, points[i].y);
		min.z = std::min(min.z, points[i].z);

		max.x = std::max(max.x, points[i].x);
		max.y = std::max(max.y, points[i].y);
		max.z = std::max(max.z, points[i].z);
	}
}

using Boxi = Box<int>;
using Boxu = Box<unsigned>;
using Boxf = Box<float>;
using Boxd = Box<double>;