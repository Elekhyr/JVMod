#include "Layersfield.hpp"


Layersfield::Layersfield(const std::string& name, const Scalarfield& field)
{
	nx = field.mScalars[0].size();
	ny = field.mScalars.size();
	mBox = field.mBox;
	mFields[name] = field;
	mNames.push_back(name);
	
}

const Boxd& Layersfield::_Box() const
{
	return mBox;
}

const double Layersfield::_HeightTotal(const int indI, const int indJ) const
{
	double height = 0;
	for (int i = 1; i < mNames.size(); i++)
	{
		height += _Field(mNames[i]).GridScalar(indI, indJ);
	}
	return height;
}

const double Layersfield::_HeightTotal(const double& x, const double& y) const
{
	double height = 0;
	for (int i = 1; i < mNames.size(); i++)
	{
		height += _Field(mNames[i]).Scalar(x, y);
	}
	return height;
}

const std::vector<Math::Vec2i> Layersfield::_Voisin4(const int i, const int j) const
{
	std::vector<Math::Vec2i> voisins;
	voisins.reserve(4);

	if (i > 0)
		voisins.push_back(Math::Vec2i(i-1, j));
	if (j > 0)
		voisins.push_back(Math::Vec2i(i, j-1));
	if (i < nx)
		voisins.push_back(Math::Vec2i(i+1, j));
	if (j < ny)
		voisins.push_back(Math::Vec2i(i, j+1));
	return voisins;
}

const std::vector<Math::Vec2i> Layersfield::_Voisin8(const int i, const int j) const
{
	std::vector<Math::Vec2i> voisins;
	voisins.reserve(8);

	if (i > 0)
		voisins.push_back(Math::Vec2i(i-1, j));
	if (j > 0)
		voisins.push_back(Math::Vec2i(i, j-1));
	if (i < nx)
		voisins.push_back(Math::Vec2i(i+1, j));
	if (j < ny)
		voisins.push_back(Math::Vec2i(i, j+1));
	
	if (i > 0 && j > 0)
		voisins.push_back(Math::Vec2i(i-1, j-1));
	if (i > 0 && j < ny)
	voisins.push_back(Math::Vec2i(i-1, j+1));
	if (i < nx && j > 0)
	voisins.push_back(Math::Vec2i(i+1, j-1));
	if (i < nx && j < ny)
	voisins.push_back(Math::Vec2i(i+1, j+1));
	
	return voisins;
}

double Layersfield::Height(const double & x, const double & y) const
{
	return 0.0;
}

double Layersfield::Height(int i, int j) const{
	
	double res = 0.;
	for (auto& field : mFields){
		res += field.second.Scalar(i, j);
	}
	return res;

}

Math::Vec3d Layersfield::Vertice(int i, int j) const
{
	double x = i / (double)nx + mBox.a.x;
	double y = j / (double)ny + mBox.a.y;
	return Math::Vec3d(x, y, Height(i,j));
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
	if (field.mScalars.size() == ny && field.mScalars[0].size() == nx)
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
	std::vector<Math::Vec2i> voisins;
	double delta_h = 0;
	//hauteur à partir de laquelle on commence à transformer
	double delta_h_0 = 0.01;
	//coefficient de transformation
	double k = 0.5;

	for (int i=0; i < nx; i++)
	{
		for (int j=0; j < ny; j++)
		{
			voisins = _Voisin4(i,j);
			double h_bedrock = _Field(mNames[0]).GridScalar(i, j);
			for (Math::Vec2i v : voisins)
			{
				delta_h += h_bedrock - _HeightTotal(v.x, v.y);
			}
			if (delta_h > delta_h_0)
			{
				double h_transfo = k*(delta_h - delta_h_0);
				mFields[mNames[0]].mScalars[j][i] = mFields[mNames[0]].GridScalar(i, j) - h_transfo;
				mFields[mNames[1]].mScalars[j][i] = mFields[mNames[1]].GridScalar(i, j) + h_transfo;
			}
		}
	}

}
