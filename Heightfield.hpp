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

	double Height(const double& x, const double& y) const override;
	double Height(const Math::Vec2d& pos) const override;
	const Boxd& _Box() const override;

	double HorizonSlope(const Math::Vec3d& pos, const Math::Vec2d& dir) const override;
	bool Visible(const Math::Vec3d& pos, const Math::Vec3d& point) const override;

	Math::Vec3d Normal(double x, double y) const;
	void ExportToObj(const std::string& path, unsigned nbPointsX, unsigned nbPointsY) const override;
};
