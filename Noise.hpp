/**
* \file Noise.hpp
* \brief todo
* \author Elekhyr
* \version 1.0
* \date 03/12/17
* \copyright Copyright (c) 2017 Thomas Margier
*  This file is licensed under the MIT License, see https://opensource.org/licenses/MIT
*/
#pragma once

class Noise
{
public:

	Noise() = default;
	virtual ~Noise() = default;


	virtual double Apply(double x, double y);
};

class PerlinNoise : public Noise
{
public:

	PerlinNoise();
	virtual ~PerlinNoise() = default;
	double Apply(double x, double y) override;
};
