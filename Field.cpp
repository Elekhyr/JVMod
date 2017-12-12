#include <cfloat>
#include <cmath>
#include "Field.hpp"

Scalarfield Field::DrainArea() const
{
	return Scalarfield();
}

std::pair<Scalarfield, Scalarfield> Field::SlopeMap() const
{
	return std::pair<Scalarfield, Scalarfield>();
}

Scalarfield Field::LightMap() const
{
	return Scalarfield();
}

Scalarfield Field::WetnessMap() const
{
	return Scalarfield();
}

Scalarfield Field::StreamPowerMap() const
{
	return Scalarfield();
}

Math::Vec2d Field::Slope(unsigned i, unsigned j) const
{
	return Math::Vec2d();
}

double Field::DrainCellArea(unsigned i, unsigned j) const
{
	return 0.0;
}

double Field::Wetness(unsigned i, unsigned j) const
{
	return 0.0;
}

double Field::StreamPower(unsigned i, unsigned j) const
{
	return 0.0;
}

double Field::Light(unsigned i, unsigned j) const
{
	return 0.0;
}

Math::Vec2d Field::Slope(const Math::Vec2u pos) const
{
	return Math::Vec2d();
}

double Field::DrainCellArea(const Math::Vec2u pos) const
{
	return 0.0;
}

double Field::Wetness(const Math::Vec2u pos) const
{
	return 0.0;
}

double Field::StreamPower(const Math::Vec2u pos) const
{
	return 0.0;
}

double Field::Light(const Math::Vec2u pos) const
{
	return 0.0;
}

double Field::Wetness(const Math::Vec2u pos) const
{
	return 0.0;
}
