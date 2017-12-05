#include "Layersfield.hpp"

const Boxd& Layersfield::_Box() const
{
	return mBox;
}

double Layersfield::Height(const double & x, const double & y) const
{
	return 0.0;
}

Math::Vec3d Layersfield::Slope(const double & x, const double & y) const
{
	return Math::Vec3d();
}

unsigned Layersfield::DrainArea(const double & x, const double & y) const
{
	return 0;
}

double Layersfield::Wetness(const double & x, const double & y) const
{
	return 0.0;
}

double Layersfield::StreamPower(const double & x, const double & y) const
{
	return 0.0;
}

double Layersfield::Light(const double & x, const double & y) const
{
	return 0.0;
}

double Layersfield::Height(const Math::Vec2d & pos) const
{
	return Height(pos.x, pos.y);
}

Math::Vec3d Layersfield::Slope(const Math::Vec2d & pos) const
{
	return Slope(pos.x, pos.y);
}

unsigned Layersfield::DrainArea(const Math::Vec2d & pos) const
{
	return DrainArea(pos.x, pos.y);
}

double Layersfield::Wetness(const Math::Vec2d & pos) const
{
	return Wetness(pos.x, pos.y);
}

double Layersfield::StreamPower(const Math::Vec2d & pos) const
{
	return StreamPower(pos.x, pos.y);
}

double Layersfield::Light(const Math::Vec2d & pos) const
{
	return Light(pos.x, pos.y);
}

void Layersfield::AddField(const std::string& name, const Scalarfield& field)
{
	mBox.a += field._Box().a;
	mBox.b += field._Box().b;
	mFields[name] = field;
	mNames.push_back(name);
}

const Scalarfield& Layersfield::_Field(const std::string& field) const
{
	const auto it = mFields.find(field);
	if (it == mFields.end())
		throw std::runtime_error("Field not found");
	return it->second;
}
