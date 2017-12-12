/**
 * \file Heightfield.hpp
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
#include "Scalarfield.hpp"

class Field {
public:
	virtual double Height(const double& x, const double& y) const = 0;
	virtual double Height(const Math::Vec2d& pos) const = 0;
	virtual const Boxd& _Box() const = 0;
	virtual Math::Vec2d Slope(int x, const int y) const = 0;
	virtual std::pair<Scalarfield, Scalarfield> SlopeMap() const = 0;

	virtual double HorizonSlope(const Math::Vec3d& pos, const Math::Vec2d& dir) const = 0;
	virtual bool Visible(const Math::Vec3d& pos, const Math::Vec3d& point) const = 0;
};
