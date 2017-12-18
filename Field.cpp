#include <cfloat>
#include <cmath>
#include "Field.hpp"

Math::Vec3d getTriangleNormal(const Math::Vec3d& a, const Math::Vec3d& b, const Math::Vec3d& c){
	
	return Math::Normalize(Math::Cross(b - a, c - a));
	
}

Math::Vec3d Field::NormalCell(unsigned i, unsigned j) const{
	
	Math::Vec3d ret(0., 0., 1.);
	
	if (i == 0){
		if (j == 0){
			ret = getTriangleNormal(Vertex(i, j), Vertex(i+1, j), Vertex(i, j+1));
		} else if (j == _SizeY() - 1) {
			ret = getTriangleNormal(Vertex(i, j), Vertex(i+1, j-1), Vertex(i+1, j))
			+ getTriangleNormal(Vertex(i, j), Vertex(i, j-1), Vertex(i+1, j-1));
		} else {
			ret = getTriangleNormal(Vertex(i, j), Vertex(i, j-1), Vertex(i+1, j-1))
			+ getTriangleNormal(Vertex(i, j), Vertex(i+1, j-1), Vertex(i+1, j))
			+ getTriangleNormal(Vertex(i, j), Vertex(i+1, j), Vertex(i, j+1));
		}
	} else if (i == _SizeX()-1){
		if (j == 0){
			ret = getTriangleNormal(Vertex(i, j), Vertex(i, j+1), Vertex(i-1, j+1))
			+ getTriangleNormal(Vertex(i, j), Vertex(i-1, j+1), Vertex(i-1, j));
		} else if (j == _SizeY() - 1) {
			ret = getTriangleNormal(Vertex(i, j), Vertex(i-1, j), Vertex(i, j-1));
		} else {
			ret = getTriangleNormal(Vertex(i, j), Vertex(i, j+1), Vertex(i-1, j+1))
			+ getTriangleNormal(Vertex(i, j), Vertex(i-1, j+1), Vertex(i-1, j))
			+ getTriangleNormal(Vertex(i, j), Vertex(i-1, j), Vertex(i, j-1));
		}
	} else if (j == 0){
		ret = getTriangleNormal(Vertex(i, j), Vertex(i-1, j+1), Vertex(i-1, j))
		+ getTriangleNormal(Vertex(i, j), Vertex(i, j+1), Vertex(i-1, j+1))
		+ getTriangleNormal(Vertex(i, j), Vertex(i+1, j), Vertex(i, j+1));
	} else if (j == _SizeY()-1){
		ret = getTriangleNormal(Vertex(i, j), Vertex(i+1, j-1), Vertex(i+1, j))
		+ getTriangleNormal(Vertex(i, j), Vertex(i, j-1), Vertex(i+1, j-1))
		+ getTriangleNormal(Vertex(i, j), Vertex(i-1, j), Vertex(i, j-1));
	} else {
		ret = getTriangleNormal(Vertex(i, j), Vertex(i-1, j+1), Vertex(i-1, j))
		+ getTriangleNormal(Vertex(i, j), Vertex(i, j+1), Vertex(i-1, j+1))
		+ getTriangleNormal(Vertex(i, j), Vertex(i+1, j), Vertex(i, j+1))
		+ getTriangleNormal(Vertex(i, j), Vertex(i+1, j-1), Vertex(i+1, j))
		+ getTriangleNormal(Vertex(i, j), Vertex(i, j-1), Vertex(i+1, j-1))
		+ getTriangleNormal(Vertex(i, j), Vertex(i-1, j), Vertex(i, j-1));
	}
	
	return Math::Normalize(ret);
}


Math::Vec3d Field::Normal(double x, double y) const
{
	double u = (x - _Box().a.x) / (_ScaleX());
	double v = (y - _Box().a.y) / (_ScaleY());
	
	if (u > 1. || u < 0. || v > 1. || u < 1.)
		return Math::Vec3d(0., 0., 1.);
	
	// Cell location within grid
	double globalv = v * (_SizeY()-1);
	double globalu = u * (_SizeX()-1);
	const unsigned row = unsigned(globalv);
	const unsigned col = unsigned(globalu);
	
	// Local coordinates within cell between [0..1]
	v = globalv - row;
	u = globalu - col;
	
	
	Math::Vec3d result;
	// Interpolation with the bottom and left cells
	if (u + v < 1) {
		Math::Vec3d n00 = NormalCell(col, row);
		result = n00;
		if (row + 1 < _SizeY()) {
			Math::Vec3d n01 = NormalCell(col, row+1);
			result -= v * n00;
			result += v * n01;
		}
		
		if (col + 1 < _SizeX()) {
		Math::Vec3d n10 = NormalCell(col+1, row);
			result -= u * n00;
			result += u * n10;
		}
	}
	else {
		Math::Vec3d n11(0.,0.,0.);
		if (col+1 < _SizeX() && row+1 < _SizeY())
			n11 = NormalCell(col, row);
		result = n11;
		if (row + 1 < _SizeY()) {
			Math::Vec3d n01 = NormalCell(col, row+1);
			result -= (1.-v) * n11;
			result += (1.-v) * n01;
		}
		
		if (col + 1 < _SizeX()) {
			Math::Vec3d n10 = NormalCell(col+1, row);
			result -= (1.-u) * n11;
			result += (1.-u) * n10;
		}
	}
	return result;
}

Math::Vec3d Field::Vertex(unsigned i, unsigned j) const{
	double x = i * _ScaleX() / (double)_SizeX() + _Box().a.x;
	double y = j * _ScaleY() / (double)_SizeY() + _Box().a.y;
	return Math::Vec3d(x, y, Height(i,j));
}

Math::Vec3d Field::Vertex(double x, double y) const{
	return Math::Vec3d(x, y, Height(x,y));
}

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
	//TODO: décaler vers el haut et s'arrêter si point avant fin HF
	if (pos.z < Height(pos.x, pos.y))
		return false;
	Math::Vec3d dir = point - pos;
	
	Math::Vec2d dir2D(dir.x, dir.y);
	
	double pente = dir.z / dir2D.Length();
	
	dir2D.Normalize();
	//TODO: Scale dir2D to be of length 1 step in the grid
	
	return pente > HorizonSlope(pos, dir2D);
}
