#define _USE_MATH_DEFINES
#include <cfloat>
#include <cmath>
#include <vector>
#include <iostream>
#include <fstream>
#include <array>
#include "Field.hpp"
#include "Vec3.hpp"

Math::Vec3d getTriangleNormal(const Math::Vec3d& a, const Math::Vec3d& b, const Math::Vec3d& c){
	
	return Math::Normalize(Math::Cross(b - a, c - a));
	
}

Math::Vec3d Field::NormalCell(unsigned i, unsigned j) const{
	
	Math::Vec3d ret(0., 0., 1.);
	
	if (i == 0){
		if (j == 0){
			ret = getTriangleNormal(VertexCell(i, j), VertexCell(i+1, j), VertexCell(i, j+1));
		} else if (j == _SizeY() - 1) {
			ret = getTriangleNormal(VertexCell(i, j), VertexCell(i+1, j-1), VertexCell(i+1, j))
			+ getTriangleNormal(VertexCell(i, j), VertexCell(i, j-1), VertexCell(i+1, j-1));
		} else {
			ret = getTriangleNormal(VertexCell(i, j), VertexCell(i, j-1), VertexCell(i+1, j-1))
			+ getTriangleNormal(VertexCell(i, j), VertexCell(i+1, j-1), VertexCell(i+1, j))
			+ getTriangleNormal(VertexCell(i, j), VertexCell(i+1, j), VertexCell(i, j+1));
		}
	} else if (i == _SizeX()-1){
		if (j == 0){
			ret = getTriangleNormal(VertexCell(i, j), VertexCell(i, j+1), VertexCell(i-1, j+1))
			+ getTriangleNormal(VertexCell(i, j), VertexCell(i-1, j+1), VertexCell(i-1, j));
		} else if (j == _SizeY() - 1) {
			ret = getTriangleNormal(VertexCell(i, j), VertexCell(i-1, j), VertexCell(i, j-1));
		} else {
			ret = getTriangleNormal(VertexCell(i, j), VertexCell(i, j+1), VertexCell(i-1, j+1))
			+ getTriangleNormal(VertexCell(i, j), VertexCell(i-1, j+1), VertexCell(i-1, j))
			+ getTriangleNormal(VertexCell(i, j), VertexCell(i-1, j), VertexCell(i, j-1));
		}
	} else if (j == 0){
		ret = getTriangleNormal(VertexCell(i, j), VertexCell(i-1, j+1), VertexCell(i-1, j))
		+ getTriangleNormal(VertexCell(i, j), VertexCell(i, j+1), VertexCell(i-1, j+1))
		+ getTriangleNormal(VertexCell(i, j), VertexCell(i+1, j), VertexCell(i, j+1));
	} else if (j == _SizeY()-1){
		ret = getTriangleNormal(VertexCell(i, j), VertexCell(i+1, j-1), VertexCell(i+1, j))
		+ getTriangleNormal(VertexCell(i, j), VertexCell(i, j-1), VertexCell(i+1, j-1))
		+ getTriangleNormal(VertexCell(i, j), VertexCell(i-1, j), VertexCell(i, j-1));
	} else {
		ret = getTriangleNormal(VertexCell(i, j), VertexCell(i-1, j+1), VertexCell(i-1, j))
		+ getTriangleNormal(VertexCell(i, j), VertexCell(i, j+1), VertexCell(i-1, j+1))
		+ getTriangleNormal(VertexCell(i, j), VertexCell(i+1, j), VertexCell(i, j+1))
		+ getTriangleNormal(VertexCell(i, j), VertexCell(i+1, j-1), VertexCell(i+1, j))
		+ getTriangleNormal(VertexCell(i, j), VertexCell(i, j-1), VertexCell(i+1, j-1))
		+ getTriangleNormal(VertexCell(i, j), VertexCell(i-1, j), VertexCell(i, j-1));
	}
	
	return Math::Normalize(ret);
}


Math::Vec3d Field::Normal(double x, double y) const
{
	double u = (x - Box().a.x) / (_ScaleX());
	double v = (y - Box().a.y) / (_ScaleY());
	
	if (u > 1. || u < 0. || v > 1. || v < 0.)
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
			n11 = NormalCell(col+1, row+1);
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

Math::Vec3d Field::VertexCell(unsigned i, unsigned j) const{
	double x = i * _ScaleX() / (double)(_SizeX()-1) + Box().a.x;
	double y = j * _ScaleY() / (double)(_SizeY()-1) + Box().a.y;
	return Math::Vec3d(x, y, HeightCell(i,j));
}

Math::Vec3d Field::Vertex(double x, double y) const{
	return Math::Vec3d(x, y, Height(x,y));
}

bool myCompare(std::pair<double, Math::Vec2i>& a, std::pair<double, Math::Vec2i>& b){
	return a.first < b.first;
}

Scalarfield Field::DrainArea() const {
	Scalarfield sf;
	
	sf.mScalars = std::vector<std::vector<double>>(_SizeY(), std::vector<double>(_SizeX(), 1.));
	
	std::vector<std::pair<double, Math::Vec2i>> sorted_heights(_SizeX() *  _SizeY());
	
	for (unsigned i = 0; i < _SizeX(); i++) {
		for (unsigned j = 0; j < _SizeY(); j++) {
			sorted_heights[i * _SizeY() + j] =  std::make_pair(HeightCell(i, j), Math::Vec2i(i,j));
		}
	}
	
	std::sort(sorted_heights.rbegin(), sorted_heights.rend(), myCompare);

	for (auto& p : sorted_heights) {
		
		std::vector<Math::Vec2u> neighbour_coords;
		std::vector<double> neighbour_slopes;
		std::vector<double> neighbour_heights_diff;
		FindNeighboursFlow(unsigned(p.second.x), unsigned(p.second.y), neighbour_coords, neighbour_slopes, neighbour_heights_diff);
		
		double total = 0.;
		
		for (auto& val : neighbour_slopes){
			total += val;
		}
		
		for (int i = 0; i < neighbour_coords.size(); ++i){
			unsigned x, y;
			x = neighbour_coords[i].x;
			y = neighbour_coords[i].y;
			
			sf.SetValue(x, y, sf.CellValue(x, y) + sf.CellValue(p.second.x, p.second.y) * neighbour_slopes[i] / total) ;
		}
	}
	
	sf.mZMin = DBL_MAX;
	sf.mZMax = -DBL_MAX;
	
	for (int j = 0; j < _SizeY(); ++j){
		for (int i = 0; i < _SizeX(); ++i){
			if (sf.CellValue(i, j) > sf.mZMax){
				sf.mZMax = sf.CellValue(i, j);
			} else if (sf.CellValue(i, j) < sf.mZMin){
				sf.mZMin = sf.CellValue(i, j);
			}
		}
	}

	return std::move(sf);
}

std::pair<Scalarfield, Scalarfield> Field::SlopeMap() const
{
	Scalarfield x_field;
	Scalarfield y_field;

	x_field.mBox = Box();
	x_field.mScaleX = _ScaleX();
	x_field.mScaleY = _ScaleY();


	y_field.mBox = Box();
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


//Wetness me: https://i.ytimg.com/vi/x2CeDY9Ywhs/maxresdefault.jpg
Scalarfield Field::WetnessMap() const
{
	// Scalarfield DrainAreaMap = DrainArea();
	// Scalarfield SlopeMap = SlopeMap();
	Scalarfield wetnessMap = Scalarfield();
	// for (unsigned i = 0; i < _SizeX(); i++) {
	// 	for (unsigned j = 0; j < _SizeY(); j++) {
	// 		WetnessMap.mScalars[i][j] = std::log(DrainAreaMap.Scalar(i,j)/
	// 										(1.+ SlopeMap.Scalar(i,j)));
	// 	}
	// }
	return wetnessMap;
}

Scalarfield Field::StreamPowerMap() const
{
	return Scalarfield();
}

Math::Vec2d Field::Slope(unsigned i, unsigned j) const
{
	Math::Vec2d n;

	if (i + 1 < _SizeX() && i > 0)
	{
		if (i > 0)
		{
			n.x = (HeightCell(j, i + 1) - HeightCell(j, i - 1)) / (2 * _ScaleX() / _SizeX());
		}
		else
		{
			n.x = (HeightCell(j, i + 1) - HeightCell(j, i)) / (2 * _ScaleX() / _SizeX());
		}

		if (j + 1 < _ScaleY())
		{
			if (j > 0)
				n.y = (HeightCell(j + 1, i) - HeightCell(j - 1, i)) / (2 * _ScaleY() / _SizeY());
			else
				n.y = (HeightCell(j + 1, i) - HeightCell(j, i)) / (2 * _ScaleY() / _SizeY());
		}
		else {
			if (j > 0)
				n.y = (HeightCell(j, i) - HeightCell(j - 1, i)) / (2 * _ScaleY() / _SizeY());
			else
				n.y = HeightCell(j, i);
		}
	}
	else
	{
		if (i > 0)
		{
			n.x = (HeightCell(j, i) - HeightCell(j, i - 1)) / (2 * _ScaleX() / _SizeX());
		}
		else
		{
			n.x = HeightCell(j, i);
		}

		if (j + 1 < _SizeY())
		{
			if (j > 0)
				n.y = (HeightCell(j + 1, i) - HeightCell(j - 1, i)) / (2 * _ScaleY() / _SizeY());
			else
				n.y = (HeightCell(j + 1, i) - HeightCell(j, i)) / (2 * _ScaleY() / _SizeY());
		}
		else {
			if (j > 0)
				n.y = (HeightCell(j, i) - HeightCell(j - 1, i)) / (2 * _ScaleY() / _SizeY());
			else
				n.y = HeightCell(j, i);
		}
	}

	return n;
}


void Field::FindNeighboursFlow(unsigned i, unsigned j, std::vector<Math::Vec2u>& NeighboursCoords,
	std::vector<double>& NeighboursSlopes, std::vector<double>& NeighboursDifHeight) const
{
	if (i > 0) {
		if (HeightCell(i,j)-HeightCell(i-1,j) > 0) {
			double difHeight = HeightCell(i,j)-HeightCell(i-1,j);
			double slope = difHeight;
			NeighboursCoords.push_back(Math::Vec2u(i-1, j));
			NeighboursSlopes.push_back(slope);
			NeighboursDifHeight.push_back(difHeight);
		}
	}
	if (j > 0) {
		if (HeightCell(i,j)-HeightCell(i, j-1) > 0) {
			double difHeight = HeightCell(i,j)-HeightCell(i, j-1);
			double slope = difHeight;
			NeighboursCoords.push_back(Math::Vec2u(i, j-1));
			NeighboursSlopes.push_back(slope);
			NeighboursDifHeight.push_back(difHeight);
		}
	}
	if (i < _SizeX()-1) {
		if (HeightCell(i,j)-HeightCell(i+1, j) > 0) {
			double difHeight = HeightCell(i,j)-HeightCell(i+1,j);
			double slope = difHeight;
			NeighboursCoords.push_back(Math::Vec2u(i+1, j));
			NeighboursSlopes.push_back(slope);
			NeighboursDifHeight.push_back(difHeight);
		}
	}
	if (j < _SizeY()-1) {
		if (HeightCell(i,j)-HeightCell(i, j+1) > 0) {
			double difHeight = HeightCell(i,j)-HeightCell(i,j+1);
			double slope = difHeight;
			NeighboursCoords.push_back(Math::Vec2u(i, j+1));
			NeighboursSlopes.push_back(slope);
			NeighboursDifHeight.push_back(difHeight);
		}
	}
	if (i > 0 && j > 0) {
		if (HeightCell(i,j)-HeightCell(i-1, j-1) > 0) {
			double difHeight = HeightCell(i,j)-HeightCell(i-1,j-1);
			double slope = difHeight * M_SQRT1_2;
			NeighboursCoords.push_back(Math::Vec2u(i-1, j-1));
			NeighboursSlopes.push_back(slope);
			NeighboursDifHeight.push_back(difHeight);
		}
	}
	if (i > 0 && j < _SizeY()-1) {
		if (HeightCell(i,j)-HeightCell(i-1, j+1) > 0) {
			double difHeight = HeightCell(i,j)-HeightCell(i-1,j+1);
			double slope = difHeight * M_SQRT1_2;
			NeighboursCoords.push_back(Math::Vec2u(i-1, j+1));
			NeighboursSlopes.push_back(slope);
			NeighboursDifHeight.push_back(difHeight);
		}
	}
	if (i < _SizeX()-1 && j > 0) {
		if (HeightCell(i,j)-HeightCell(i+1, j-1) > 0) {
			double difHeight = HeightCell(i,j)-HeightCell(i+1,j-1);
			double slope = difHeight * M_SQRT1_2;
			NeighboursCoords.push_back(Math::Vec2u(i+1, j-1));
			NeighboursSlopes.push_back(slope);
			NeighboursDifHeight.push_back(difHeight);
		}
	}
	if (i < _SizeX()-1 && j < _SizeX()-1) {
		if (HeightCell(i,j)-HeightCell(i+1, j+1) > 0) {
			double difHeight = HeightCell(i,j)-HeightCell(i+1,j+1);
			double slope = difHeight * M_SQRT1_2;
			NeighboursCoords.push_back(Math::Vec2u(i+1, j+1));
			NeighboursSlopes.push_back(slope);
			NeighboursDifHeight.push_back(difHeight);
		}
	}
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
	
	const Boxd b = Box();
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
	//TODO: décaler vers le haut et s'arrêter si point avant fin HF
	if (pos.z < Height(pos.x, pos.y))
		return false;
	Math::Vec3d dir = point - pos;
	
	Math::Vec2d dir2D(dir.x, dir.y);
	
	double pente = dir.z / dir2D.Length();
	
	dir2D.Normalize();
	//TODO: Scale dir2D to be of length 1 step in the grid
	
	return pente > HorizonSlope(pos, dir2D);
}

void Field::ExportToObj(const std::string & path, unsigned nbPointsX, unsigned nbPointsY) const
{
	std::vector<std::array<unsigned, 3>> faces;
	faces.reserve(nbPointsX * nbPointsY * 2);


	std::vector<std::array<double, 3>> normals;
	normals.reserve(nbPointsX * nbPointsY);

	unsigned n = 0;
	unsigned i = 0;

	std::ofstream file;
	file.exceptions(std::ofstream::badbit | std::ofstream::failbit);

	try
	{
		file.open(path);
	}
	catch (std::ofstream::failure& e)
	{
		std::cerr << e.what() << std::endl;
	}

	if (file.is_open())
	{
		file << "g heightfield\n\n";

		const double step_x = _ScaleX() / double(nbPointsX);
		const double step_y = _ScaleY() / double(nbPointsY);
		for (double x = Box().a.x; i < nbPointsX; x += step_x, ++i)
		{
			unsigned j = 0;
			for (double y = Box().a.y; j < nbPointsY; y += step_y, ++j)
			{
				double z = Height(i, j);
				file << "v " << x << " " << y << " " << z << "\n";
				const auto normal = Math::Vec3d();// Normal(x, y);
				normals.push_back({ normal.x, normal.y, normal.z });

				if (i < nbPointsX - 1 && j < nbPointsY - 1)
				{
					faces.push_back({ n + j + 2,
						n + j + 1,
						n + j + nbPointsX + 1 });
					faces.push_back({ n + j + nbPointsX + 1,
						n + j + nbPointsX + 2,
						n + j + 2 });
				}
			}
			n += j;
		}

		file << "\n\n";
		for (auto& n : normals)
		{
			file << "vn " << n[0] << " " << n[1] << " " << n[2] << " \n";
		}


		file << "\n\n";
		for (auto& f : faces)
		{
			file << "f "
				<< f[0] << "//" << f[0] << " "
				<< f[1] << "//" << f[1] << " "
				<< f[2] << "//" << f[2] << " \n";
		}

		file.close();
	}
}
