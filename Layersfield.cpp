#include "Layersfield.hpp"


Layersfield::Layersfield(const std::string& name, const Scalarfield& field)
{
	mNX = field.mScalars[0].size();
	mNY = field.mScalars.size();
	mBox = field.mBox;
	mFields[name] = field;
	mNames.push_back(name);
	
}

const Boxd& Layersfield::_Box() const
{
	return mBox;
}

unsigned Layersfield::_SizeX() const
{
	return mNX;
}

unsigned Layersfield::_SizeY() const
{
	return mNY;
}

unsigned Layersfield::_ScaleX() const
{
	return 0;
}

unsigned Layersfield::_ScaleY() const
{
	return 0;
}

const std::vector<Math::Vec2u> Layersfield::_Voisin4(const unsigned i, const unsigned j) const
{
	std::vector<Math::Vec2u> voisins;
	voisins.reserve(4);

	if (i > 0)
		voisins.push_back(Math::Vec2u(i-1, j));
	if (j > 0)
		voisins.push_back(Math::Vec2u(i, j-1));
	if (i < mNX)
		voisins.push_back(Math::Vec2u(i+1, j));
	if (j < mNY)
		voisins.push_back(Math::Vec2u(i, j+1));
	return voisins;
}

const std::vector<Math::Vec2u> Layersfield::_Voisin8(const unsigned i, const unsigned j) const
{
	std::vector<Math::Vec2u> voisins;
	voisins.reserve(8);

	if (i > 0)
		voisins.push_back(Math::Vec2u(i-1, j));
	if (j > 0)
		voisins.push_back(Math::Vec2u(i, j-1));
	if (i < mNX)
		voisins.push_back(Math::Vec2u(i+1, j));
	if (j < mNY)
		voisins.push_back(Math::Vec2u(i, j+1));
	
	if (i > 0 && j > 0)
		voisins.push_back(Math::Vec2u(i-1, j-1));
	if (i > 0 && j < mNY)
	voisins.push_back(Math::Vec2u(i-1, j+1));
	if (i < mNX && j > 0)
	voisins.push_back(Math::Vec2u(i+1, j-1));
	if (i < mNX && j < mNY)
	voisins.push_back(Math::Vec2u(i+1, j+1));
	
	return voisins;
}

double Layersfield::Height(const double & x, const double & y) const
{
	double height = 0;
	for (auto& field : mFields) {
		height += field.second.Scalar(x, y);
	}
	return height;
}

double Layersfield::Height(unsigned i, unsigned j) const {
	
	double res = 0.;
	for (auto& field : mFields){
		res += field.second.Scalar(i, j);
	}
	return res;
}

Math::Vec3d Layersfield::Vertex(unsigned i, unsigned j) const
{
	double x = i / (double)mNX + mBox.a.x;
	double y = j / (double)mNY + mBox.a.y;
	return Math::Vec3d(x, y, Height(i,j));
}

double Layersfield::Height(const Math::Vec2d & pos) const
{
	return Height(pos.x, pos.y);
}

Math::Vec3d Layersfield::Normal(unsigned i, unsigned j) const
{
	return Math::Vec3d();
}

void Layersfield::AddField(const std::string& name, const Scalarfield& field)
{
	if (field.mScalars.size() == mNY && field.mScalars[0].size() == mNX)
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
	std::vector<Math::Vec2u> voisins;
	double delta_h = 0;
	//hauteur à partir de laquelle on commence à transformer
	double delta_h_0 = 0.01;
	//coefficient de transformation
	double k = 0.5;

	for (unsigned i = 0; i < mNX; i++)
	{
		for (unsigned j = 0; j < mNY; j++)
		{
			voisins = _Voisin4(i,j);
			double h_bedrock = _Field(mNames[0]).Scalar(i, j);
			for (Math::Vec2u v : voisins)
			{
				delta_h += h_bedrock - Height(v.x, v.y);
			}
			if (delta_h > delta_h_0)
			{
				double h_transfo = k*(delta_h - delta_h_0);
				mFields[mNames[0]].mScalars[j][i] = mFields[mNames[0]].Scalar(i, j) - h_transfo;
				mFields[mNames[1]].mScalars[j][i] = mFields[mNames[1]].Scalar(i, j) + h_transfo;
			}
		}
	}

}
