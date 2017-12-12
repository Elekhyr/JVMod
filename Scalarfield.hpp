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
#include <vector>
#include "Vec2.hpp"
#include "Box.hpp"

enum class Color
{
	Gray,
	Red,
	Blue,
	Green,
	Purple,
	Yellow,
	Cyan
};

class Scalarfield
{
public:
	Scalarfield() = default;
	Scalarfield(const std::string& imagePath, const Boxd& boudingBox, double zmin, double zmax);

	const Boxd& _Box() const;

	double GridScalar(int i, int j) const;
	double Scalar(const double& x, const double& y) const;
	double Scalar(int i, int j) const;
	Math::Vec3d Vertice(int i, int j) const;
	unsigned GridXIndex(const double& x) const;
	unsigned GridYIndex(const double& y) const;

	virtual void ExportToObj(const std::string& path, unsigned nbPointsX, unsigned nbPointsY) const;
	void Save(const std::string& path, const Color& color = Color::Gray);
protected:

	friend class Layersfield;
	friend class Heightfield;
	
	//boite englobante
	Boxd mBox;
	
	//taille de la boite
	double mScaleX;
	double mScaleY;

	//intervalle du scalaire
	double mZMin;
	double mZMax;
	
	//tableau des scalaires
	std::vector<std::vector<double>> mScalars;

	//taille d'une case
	int nx;
	int ny;

private:
	double BilinearInterpolation(unsigned row, unsigned col, const double& u, const double& v) const;
};
