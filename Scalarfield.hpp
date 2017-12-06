/**
* \file Scalarfield.hpp
* \brief todo
* \authors Crymsius, Elekhyr, Plopounet
* \version 1.0
* \date 02/12/17
* \copyright Copyright (c) 2017 Romain Maurizot, Thomas Margier, Loïs Paulin
*  This file is licensed under the MIT License, see https://opensource.org/licenses/MIT
*/
#pragma once
#include "Vec2.hpp"
#include "Box.hpp"
#include <vector>

class Scalarfield
{
public:
	Scalarfield();
	Scalarfield(const std::string& imagePath, const Boxd& boudingBox, double zmin, double zmax);

	double GridScalar(int i, int j) const;
	double Scalar(const double& x, const double& y) const;

	void ExportToObj(const std::string& path, unsigned nbPointsX, unsigned nbPointsY) const;
	void Save(const std::string& path);
protected:

	Boxd mBox;
	double mScaleX;
	double mScaleY;

	double mMin;
	double mMax;
	std::vector<std::vector<double>> mScalars;

private:
	double BilinearInterpolation(unsigned row, unsigned col, const double& u, const double& v) const;
};
