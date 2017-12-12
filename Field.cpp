#include <cfloat>
#include <cmath>
#include "Field.hpp"

Scalarfield Field::DrainArea() const
{
	return Scalarfield();
}

std::pair<Scalarfield, Scalarfield> Field::SlopeMap() const
{
	Scalarfield x_field;
	Scalarfield y_field;

	x_field.mBox = _Box();
	x_field.mScaleX = _ScaleX();
	x_field.mScaleY = _ScaleY();


	y_field.mBox = _Box();
	y_field.mScaleX = _ScaleX();
	y_field.mScaleY = _ScaleY();

	x_field.mScalars = std::vector<std::vector<double>>(_SizeY(), std::vector<double>(_SizeX()));
	y_field.mScalars = std::vector<std::vector<double>>(_SizeY(), std::vector<double>(_SizeX()));

	double min_x = 0;
	double min_y = 0;
	double max_x = 0;
	double max_y = 0;

	for (unsigned j = 0; j < _SizeY(); ++j)
	{
		for (unsigned i = 0; i < _SizeX(); ++i)
		{
			auto slope = Slope(i, j);
			x_field.mScalars[j][i] = slope.x;
			y_field.mScalars[j][i] = slope.y;

			min_x = std::min(min_x, slope.x);
			min_y = std::min(min_y, slope.y);
			max_x = std::max(max_x, slope.x);
			max_y = std::max(max_y, slope.y);

		}
	}

	x_field.mZMax = max_x;
	x_field.mZMin = min_x;

	y_field.mZMax = max_y;
	y_field.mZMin = min_y;

	return std::move(std::make_pair(std::move(x_field), std::move(y_field)));
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
	return Slope(pos.x, pos.y);
}

double Field::DrainCellArea(const Math::Vec2u pos) const
{
	return DrainCellArea(pos.x, pos.y);
}

double Field::Wetness(const Math::Vec2u pos) const
{
	return Wetness(pos.x, pos.y);
}

double Field::StreamPower(const Math::Vec2u pos) const
{
	return StreamPower(pos.x, pos.y);
}

double Field::Light(const Math::Vec2u pos) const
{
	return Light(pos.x, pos.y);
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
