#include <fstream>
#include <iostream>
#include <array>
#include "Heightfield.hpp"

double Heightfield::Height(const double& x, const double& y) const
{
	return Value(x, y);
}

double Heightfield::HeightCell(unsigned i, unsigned j) const
{
	return Value(i, j);
}

double Heightfield::Height(const Math::Vec2d& pos) const
{
	return Value(pos.x, pos.y);
}

unsigned Heightfield::_SizeX() const
{
	return unsigned(mScalars[0].size());
}

unsigned Heightfield::_SizeY() const
{
	return unsigned(mScalars.size());
}

double Heightfield::_ScaleX() const
{
	return mScaleX;
}

double Heightfield::_ScaleY() const
{
	return mScaleY;
}

const Boxd& Heightfield::Box() const
{
	return _Box();
}
