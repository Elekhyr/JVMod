#include "Multilayerfield.hpp"

void Multilayerfield::AddField(const std::string& name, const Scalarfield& field)
{
	mFields[name] = field;
}

const Scalarfield& Multilayerfield::_Field(const std::string& field) const
{
	const auto it = mFields.find(field);
	if (it == mFields.end())
		throw std::exception("Field not found");
	return it->second;
}
