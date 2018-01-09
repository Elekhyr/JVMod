/**
* \file Layersfield.hpp
* \brief todo
* \authors Crymsius, Elekhyr, Plopounet
* \version 1.0
* \date 02/12/17
* \copyright Copyright (c) 2017 Romain Maurizot, Thomas Margier, Loïs Paulin
*  This file is licensed under the MIT License, see https://opensource.org/licenses/MIT
*/
#pragma once
#include "Box.hpp"
#include "SimplexNoise.hpp"

class AnalyticHeightField
{
public:
	virtual ~AnalyticHeightField() = default;
	AnalyticHeightField();
	AnalyticHeightField(const Boxd& boudingBox, const double amplitude, const double lambda, const unsigned level);

	double SimplexNoiseAt(const Math::Vec2<double> v);
	// virtual void Noisify(Scalarfield& field, const unsigned short level, const unsigned amplitude, const unsigned length) final;
	// virtual double At(const Math::Vec2d& p) const = 0;
protected:
	//boite englobante
	Boxd mBox;

	//taille de la boite
	double mScaleX;
	double mScaleY;

	//amplitude verticale
	double mAmplitude;
	//longueur d'onde max
	double mLambda;
	//nombre de niveaux
	unsigned mLevel;
};