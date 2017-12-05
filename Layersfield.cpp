#include "Layersfield.hpp"

const Boxd& Layersfield::_Box() const
{
	return mBox;
}
const Scalarfield& Layersfield::_HighestFieldGrid(const int indI, const int indJ) const
{
	int highestLayerInd = 0;
	for (int i = 1; i < mNames.size(); i++)
	{
		if (this->_Field(mNames[i]).GridScalar(indI, indJ) >= this->_Field(mNames[highestLayerInd]).GridScalar(indI,indJ))
			highestLayerInd = i;
	}
	return this->_Field(mNames[highestLayerInd]);
}

const Scalarfield& Layersfield::_HighestField(const double& x, const double& y) const
{
	int highestLayerInd = 0;
	for (int i = 1; i < mNames.size(); i++)
	{
		if (this->_Field(mNames[i]).Scalar(x, y) >= this->_Field(mNames[highestLayerInd]).Scalar(x,y))
			highestLayerInd = i;
	}
	return this->_Field(mNames[highestLayerInd]);
}


void Layersfield::AddField(const std::string& name, const Scalarfield& field)
{
	mBox.a += field._Box().a;
	mBox.b += field._Box().b;
	mFields[name] = field;
	mNames.push_back(name);
}

const Scalarfield& Layersfield::
_Field(const std::string& field) const
{
	const auto it = mFields.find(field);
	if (it == mFields.end())
		throw std::runtime_error("Field not found");
	return it->second;
}

void Layersfield::Thermal(const int temp)
{
	for (int i = mBox.a.x; i < mBox.a.y; i++) {
		for (int j = mBox.b.x; j < mBox.b.y; j++) {
			//WIP
		}
	}
}
