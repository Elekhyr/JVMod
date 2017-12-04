#include "Layersfield.hpp"

void Layersfield::AddField(const std::string& name, const Scalarfield& field)
{
	mFields[name] = field;
}

const Scalarfield& Layersfield::_Field(const std::string& field) const
{
	const auto it = mFields.find(field);
	if (it == mFields.end())
		throw std::runtime_error("Field not found");
	return it->second;
}
