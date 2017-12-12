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
#include <unordered_map>
#include "Scalarfield.hpp"
#include "Field.hpp"
#include "Vec2.hpp"

class Layersfield : public Field
{
public:
	Layersfield() = default;
	Layersfield(const std::string& name, const Scalarfield& field);

	void AddField(const std::string& name, const Scalarfield& field);
	const Scalarfield& _Field(const std::string& field) const;
	const Boxd& _Box() const;

	const std::vector<Math::Vec2i> _Voisin4(const int i, const int j) const;
	const std::vector<Math::Vec2i> _Voisin8(const int i, const int j) const;

	void Thermal(const int temp);
	double Height(const double& x, const double& y) const;
	double Height(int i, int j) const;
	Math::Vec3d Vertice(int i, int j) const;
	Math::Vec3d Slope(const double& x, const double& y) const;
	unsigned DrainArea(const double& x, const double& y) const;
	double Wetness(const double& x, const double& y) const;
	double StreamPower(const double& x, const double& y) const;
	double Light(const double& x, const double& y) const;

	double Height(const Math::Vec2d& pos) const;
	Math::Vec3d Slope(const Math::Vec2d& pos) const;
	unsigned DrainArea(const Math::Vec2d& pos) const;
	double Wetness(const Math::Vec2d& pos) const;
	double StreamPower(const Math::Vec2d& pos) const;
	double Light(const Math::Vec2d& pos) const;

private:
	std::unordered_map<std::string, Scalarfield> mFields;
	std::vector<std::string> mNames;
	Boxd mBox;
	int nx;
	int ny;
	//TODO: add construct
	double deltax;
	double deltay;
};
