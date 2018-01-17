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
	Layersfield(unsigned nx, unsigned ny, Boxd box);
	Layersfield(const std::string& name, const Scalarfield& field, const Math::Vec3d& color = Math::Vec3d(0., 0., 1.));

	void AddField(const std::string& name, const Scalarfield& field, const Math::Vec3d& color = Math::Vec3d(1., 0., 1.));
	void AddEmptyField(const std::string& name, const Math::Vec3d& color = Math::Vec3d(1.,0.,1.));
	const Scalarfield& _Field(const std::string& field) const;
	const Boxd& Box() const override;

	std::vector<Math::Vec2u> _Voisin4(const unsigned i, const unsigned j) const;
	std::vector<Math::Vec2u> _Voisin8(const unsigned i, const unsigned j) const;

	// Applique un coup de température qui transforme la bedrock en sand
	void Thermal(const int temp = 30);
	// répartie le sand sur la bedrock si instable
	void Stabilize();
	double Height(const double& x, const double& y) const override;
	double Height(const Math::Vec2d& pos) const override;
	double HeightCell(unsigned i, unsigned j) const override;

	unsigned _SizeX() const override;
	unsigned _SizeY() const override;
	double _ScaleX() const override;
	double _ScaleY() const override;
	
	void Save(const std::string& path);

private:
	std::unordered_map<std::string, Scalarfield> mFields;
	std::unordered_map<std::string, Math::Vec3d> mColors;
	std::vector<std::string> mNames;
	Boxd mBox;
	unsigned mNX;
	unsigned mNY;
	
	double mDeltaX;
	double mDeltaY;
};
