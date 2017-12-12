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
double Field::HorizonSlope(const Math::Vec3d& pos, const Math::Vec2d& dir) const{
	
	const Boxd b = _Box();
	Math::Vec2d actPos = Math::Vec2d(pos.x, pos.y) + dir;
	double sizeStep = dir.Length();
	double nbStep = sizeStep;
	double pente = -DBL_MAX;
	
	while(b.IsInside(actPos)){
		double h = Height(actPos.x, actPos.y) - pos.z;
		double tmp = h / nbStep;
		
		if (tmp > pente){
			pente = tmp;
		}
		
		actPos += dir;
		nbStep += sizeStep;
	}
	
	return pente;
	
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
bool Field::Visible(const Math::Vec3d& pos, const Math::Vec3d& point) const{
	if (pos.z < Height(pos.x, pos.y))
		return false;
	Math::Vec3d dir = point - pos;
	
	Math::Vec2d dir2D(dir.x, dir.y);
	
	double pente = dir.z / dir2D.Length();
	
	dir2D.Normalize();
	//TODO: Scale dir2D to be of length 1 step in the grid
	
	return pente > HorizonSlope(pos, dir2D);
}
