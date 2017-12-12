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
#include "Vec3.hpp"
#include "Box.hpp"
#include "Scalarfield.hpp"

class Field {
public:
	// Abstract methods
	virtual double Height(const double& x, const double& y) const = 0;
	virtual double Height(const Math::Vec2d& pos) const = 0;
	virtual Math::Vec3d Normal(unsigned i, unsigned j) const = 0;
	virtual Math::Vec3d Vertex(unsigned i, unsigned j) const = 0;
	virtual double Height(unsigned i, unsigned j) const = 0;

	virtual const Boxd& _Box() const = 0;
	virtual unsigned _SizeX() const = 0;
	virtual unsigned _SizeY() const = 0;
	virtual unsigned _ScaleX() const = 0;
	virtual unsigned _ScaleY() const = 0;


	double HorizonSlope(const Math::Vec3d& pos, const Math::Vec2d& dir) const;
	bool Visible(const Math::Vec3d& pos, const Math::Vec3d& point) const;

	/**
	 * Compute the drain area of the field
	 */
	virtual Scalarfield DrainArea() const;
	
	/**
	 * Compute the slope map
	 */
	virtual std::pair<Scalarfield, Scalarfield> SlopeMap() const;

	/**
	 * Compute the lighting map
	 */
	virtual Scalarfield LightMap() const;

	/**
	 * Compute the wetness map
	 */
	virtual Scalarfield WetnessMap() const;

	/**
	 * Compute the stream power map
	 */
	virtual Scalarfield StreamPowerMap() const;

protected:
	virtual Math::Vec2d Slope(unsigned i, unsigned j) const;
	virtual double DrainCellArea(unsigned i, unsigned j) const;
	virtual double Wetness(unsigned i, unsigned j) const;
	virtual double StreamPower(unsigned i, unsigned j) const;
	virtual double Light(unsigned i, unsigned j) const;

	virtual Math::Vec2d Slope(const Math::Vec2u pos) const;
	virtual double DrainCellArea(const Math::Vec2u pos) const;
	virtual double Wetness(const Math::Vec2u pos) const;
	virtual double StreamPower(const Math::Vec2u pos) const;
	virtual double Light(const Math::Vec2u pos) const;

};
