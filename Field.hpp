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
#include "Vec3.hpp"
#include "Vec2.hpp"

class Field {
	virtual double Height(const double& x, const double& y) const = 0;
	virtual double Height(const Math::Vec2d& pos) const = 0;
};
