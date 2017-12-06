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
#include "Scalarfield.hpp"
#include "Noise.hpp"

class Heightfield
	: public Scalarfield
{
public:

	Heightfield(const std::string& imagePath, const Boxd& boudingBox, const double zmin, const double zmax,
		const std::vector<std::vector<double>>& alphaMap, const Noise& noise);
	


};
