#pragma once
#include <algorithm>
#include <type_traits>
#include "Vec3.hpp"
#include "Vec2.hpp"

template <class T>
struct Box
{
	static_assert(std::is_arithmetic<T>::value, "T must be numeric");
	Box();
	explicit Box(const Math::Vec3<T> points[]);

	Box(const Box& other)
		: a(other.a),
		  b(other.b)
	{
	}

	Box(Box&& other) noexcept
		: a(std::move(other.a)),
		  b(std::move(other.b))
	{
	}

	Box& operator=(const Box& other)
	{
		if (this == &other)
			return *this;
		a = other.a;
		b = other.b;
		return *this;
	}

	Box& operator=(Box&& other) noexcept
	{
		if (this == &other)
			return *this;
		a = std::move(other.a);
		b = std::move(other.b);
		return *this;
	}

	Box& operator+=(const Box& other) noexcept
	{
		a = min(a, other.a);
		b = max(b, other.b);
		return *this;
	}

	void Boundaries(const Math::Vec3<T> points[]) const;
	
	bool IsInside(const Math::Vec3<T>& point) const;

	bool IsInside(const Math::Vec2<T>& pos) const;
	
	Math::Vec3<T> a;
	Math::Vec3<T> b;
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
void Box<T>::Boundaries(const Math::Vec3<T> points[]) const
{
	for (unsigned i = 0; i < sizeof(points); ++i)
	{
		a.x = std::min(a.x, points[i].x);
		a.y = std::min(a.y, points[i].y);
		a.z = std::min(a.z, points[i].z);

		b.x = std::max(b.x, points[i].x);
		b.y = std::max(b.y, points[i].y);
		b.z = std::max(b.z, points[i].z);
	}
}

template<class T>
bool Box<T>::IsInside(const Math::Vec3<T>& point) const
{
	return point.x <= b.x && point.x >= a.x
		&& point.y <= b.y && point.y >= a.y
		&& point.z <= b.z && point.z >= a.z;
}

template <class T>
bool Box<T>::IsInside(const Math::Vec2<T>& pos) const{
	return (pos.x >= a.x && pos.x <= b.x
		 && pos.y >= a.y && pos.y <= b.y);
}

using Boxi = Box<int>;
using Boxu = Box<unsigned>;
using Boxf = Box<float>;
using Boxd = Box<double>;
