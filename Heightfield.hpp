﻿/**
* \file Heightfield.hpp
* \brief todo
* \authors Crymsius, Elekhyr, Plopounet
* \version 1.0
* \date 02/12/17
* \copyright Copyright (c) 2017 Romain Maurizot, Thomas Margier, Loïs Paulin
*  This file is licensed under the MIT License, see https://opensource.org/licenses/MIT
*/
#pragma once
#include "Field.hpp"
#include "Scalarfield.hpp"
#include "Vec3.hpp"
#include "Vec2.hpp"

class Heightfield
	: public Field, public Scalarfield
{
public:
	Heightfield(const std::string& imagePath, const Boxd& boudingBox, double zmin, double zmax)
		: Scalarfield(imagePath, boudingBox, zmin, zmax)
	{
	}
	
	virtual double Height(const double& x, const double& y) const override;
	virtual double Height(const Math::Vec2d& pos) const override;
	virtual Math::Vec3d Normal(unsigned i, unsigned j) const override;
	virtual Math::Vec3d Vertex(unsigned i, unsigned j) const override;
	virtual double Height(unsigned i, unsigned j) const override;

	virtual const Boxd& _Box() const override;
	virtual unsigned _SizeX() const override;
	virtual unsigned _SizeY() const override;
	unsigned _ScaleX() const override;
	unsigned _ScaleY() const override;

	Math::Vec3d Normal(double x, double y) const;
	void ExportToObj(const std::string& path, unsigned nbPointsX, unsigned nbPointsY) const override;
};
