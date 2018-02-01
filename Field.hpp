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
	virtual ~Field() = default;
	// Abstract methods
	virtual double Height(const double& x, const double& y) const = 0;
	virtual double Height(const Math::Vec2d& pos) const = 0;
	virtual double HeightCell(unsigned i, unsigned j) const = 0;

	virtual const Boxd& Box() const = 0;
	virtual unsigned _SizeX() const = 0;
	virtual unsigned _SizeY() const = 0;
	virtual double _ScaleX() const = 0;
	virtual double _ScaleY() const = 0;
	
	Math::Vec3d NormalCell(unsigned i, unsigned j) const;
	Math::Vec3d Normal(double x, double y) const;
	Math::Vec3d VertexCell(unsigned i, unsigned j) const;
	Math::Vec3d Vertex(double x, double y) const;
	

	double HorizonSlope(const Math::Vec3d& pos, const Math::Vec2d& dir) const;
	bool Visible(const Math::Vec3d& pos, const Math::Vec3d& point) const;

	void ExportToObj(const std::string & path, unsigned nbPointsX, unsigned nbPointsY) const;

	/**
	 * Compute the drain area of the field
	 */
	Scalarfield DrainArea() const;
	
	/**
	 * Compute the slope map
	 */
	Scalarfield SlopeMap() const;

	/**
	 * Compute the lighting map
	 */
	Scalarfield LightMap() const;

	/**
	 * Compute the wetness map
	 */
	Scalarfield WetnessMap() const;

	/**
	 * Compute the stream power map
	 */
	Scalarfield StreamPowerMap() const;

protected:
	Math::Vec2d Slope(unsigned i, unsigned j) const;
	virtual void FindNeighboursFlow(unsigned i, unsigned j, std::vector<Math::Vec2u>& NeighboursCoords,
		std::vector<double>& NeighboursSlopes, std::vector<double>& NeighboursDifHeight) const;
	virtual double Wetness(unsigned i, unsigned j) const;
	virtual double StreamPower(unsigned i, unsigned j) const;
	virtual double Light(unsigned i, unsigned j) const;

	virtual Math::Vec2d Slope(const Math::Vec2u pos) const;
	virtual double Wetness(const Math::Vec2u pos) const;
	virtual double StreamPower(const Math::Vec2u pos) const;
	virtual double Light(const Math::Vec2u pos) const;

};

Math::Vec3d getTriangleNormal(const Math::Vec3d& a, const Math::Vec3d& b, const Math::Vec3d& c);
