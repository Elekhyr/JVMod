/**
* \file Multilayerfield.hpp
* \brief todo
* \authors Crymsius, Elekhyr, Plopounet
* \version 1.0
* \date 02/12/17
* \copyright Copyright (c) 2017 Romain Maurizot, Thomas Margier, Loïs Paulin
*  This file is licensed under the MIT License, see https://opensource.org/licenses/MIT
*/
#pragma once
#include <unordered_map>
#include "Field.hpp"
#include "Scalarfield.hpp"

class Multilayerfield : public Field
{
public:

	void AddField(const std::string& name, const Scalarfield& field);
	const Scalarfield& _Field(const std::string& field) const;

private:
	std::unordered_map<std::string, Scalarfield> mFields;
	
};
