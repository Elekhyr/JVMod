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
#include "AnalyticHeightField.hpp"

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
	virtual ~Scalarfield() = default;
	Scalarfield();
	Scalarfield(const std::string& imagePath, const Boxd& boudingBox, double zmin, double zmax);
	Scalarfield(const Boxd& boudingBox, double zmin, double zmax, double nbX, double nbY);
	const Boxd& _Box() const;

	double Value(const double& x, const double& y) const;
	double CellValue(unsigned i, unsigned j) const;
	Math::Vec3d Vertice(unsigned i, unsigned j) const;
	unsigned GridXIndex(const double& x) const;
	unsigned GridYIndex(const double& y) const;

	virtual void ExportToObj(const std::string& path, unsigned nbPointsX, unsigned nbPointsY) const;
	void Save(const std::string& path, const Color& color = Color::Gray);

	void SetValue(const unsigned i, const unsigned j, const double value);

	void ScalarFromNoise(AnalyticHeightField& analyticHeightField);

	const Scalarfield& operator+= (const Scalarfield& sf);

protected:
	friend class Field;
	friend class Noise;
	friend class SimplexNoise;
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
	unsigned mNX;
	unsigned mNY;

private:
	double BilinearInterpolation(unsigned row, unsigned col, const double& u, const double& v) const;
};
