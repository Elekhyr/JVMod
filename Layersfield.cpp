#include "Layersfield.hpp"
#define STB_IMAGE_WRITE_STATIC
#include "stb_image_write.h"

#define VISIBILITY_EPSILON 0.001

Layersfield::Layersfield(unsigned nx, unsigned ny, Boxd box)
	: mBox(box), mNX(nx), mNY(ny), mDeltaX(0), mDeltaY(0)
{
}

Layersfield::Layersfield(const std::string& name, const Scalarfield& field, const Math::Vec3d& color): mDeltaX(0), mDeltaY(0)
{
	mNX = static_cast<int>(field.mScalars[0].size());
	mNY = static_cast<int>(field.mScalars.size());
	mBox = field.mBox;
	mFields[name] = field;
	mColors[name]= color;
	mNames.push_back(name);
}

const Boxd& Layersfield::Box() const
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

double Layersfield::_ScaleX() const
{
	return mBox.b.x - mBox.a.x;
}

double Layersfield::_ScaleY() const
{
	return mBox.b.y - mBox.a.y;
}

std::vector<Math::Vec2u> Layersfield::_Voisin4(const unsigned i, const unsigned j) const
{
	std::vector<Math::Vec2u> voisins;
	voisins.reserve(4);

	if (i > 0)
		voisins.push_back(Math::Vec2u(i-1, j));
	if (j > 0)
		voisins.push_back(Math::Vec2u(i, j-1));
	if (i < mNX-1)
		voisins.push_back(Math::Vec2u(i+1, j));
	if (j < mNY-1)
		voisins.push_back(Math::Vec2u(i, j+1));
	return voisins;
}

std::vector<Math::Vec2u> Layersfield::_Voisin8(const unsigned i, const unsigned j) const
{
	std::vector<Math::Vec2u> voisins;
	voisins.reserve(8);

	if (i > 0)
		voisins.push_back(Math::Vec2u(i-1, j));
	if (j > 0)
		voisins.push_back(Math::Vec2u(i, j-1));
	if (i < mNX-1)
		voisins.push_back(Math::Vec2u(i+1, j));
	if (j < mNY-1)
		voisins.push_back(Math::Vec2u(i, j+1));
	
	if (i > 0 && j > 0)
		voisins.push_back(Math::Vec2u(i-1, j-1));
	if (i > 0 && j < mNY-1)
	voisins.push_back(Math::Vec2u(i-1, j+1));
	if (i < mNX-1 && j > 0)
	voisins.push_back(Math::Vec2u(i+1, j-1));
	if (i < mNX-1 && j < mNY-1)
	voisins.push_back(Math::Vec2u(i+1, j+1));
	
	return voisins;
}

double Layersfield::Height(const double & x, const double & y) const
{
	double height = 0;
	for (auto& field : mFields) {
		height += field.second.Value(x, y);
	}
	return height;
}

double Layersfield::HeightCell(unsigned i, unsigned j) const {
	
	double res = 0.;
	for (auto& field : mFields){
		res += field.second.CellValue(i, j);
	}
	return res;
}

double Layersfield::Height(const Math::Vec2d & pos) const
{
	return Height(pos.x, pos.y);
}

void Layersfield::AddField(const std::string& name, const Scalarfield& field, const Math::Vec3d& color)
{
	if(field.mNY == mNY && field.mNX == mNX && field._Box().a == mBox.a && field._Box().b == mBox.b){
		mFields[name] = field;
		mColors[name] = color;
		mNames.push_back(name);
	}
}

void Layersfield::AddEmptyField(const std::string& name, const Math::Vec3d& color)
{
	Scalarfield sf = Scalarfield(Box(), 0., 100.,mNX,mNY);
	AddField(name, sf, color);
}

const Scalarfield& Layersfield::_Field(const std::string& field) const
{
	const auto it = mFields.find(field);
	if (it == mFields.end())
		throw std::runtime_error("Field not found");
	return it->second;
}

void Layersfield::Thermal(const int temp)
{
	double delta_h = 0.;
	//hauteur à partir de laquelle on commence à transformer
	const double delta_h_0 = 0.1;

	//coefficient de transformation
	const double k = 15;

	for (unsigned j = 0; j < mNY; j++)
	{
		for (unsigned i = 0; i < mNX; i++)
		{
			std::vector<Math::Vec2u> voisins = _Voisin4(i,j);
			const double h_bedrock = _Field(mNames[0]).CellValue(i, j);
			for (auto& v : voisins)
			{
				delta_h += std::max((h_bedrock - HeightCell(v.x, v.y)),0.);
			}
			delta_h /= voisins.size();
			if (delta_h > delta_h_0)
			{
				const double h_transfo = k*(delta_h - delta_h_0);
				mFields[mNames[0]].mScalars[j][i] = mFields[mNames[0]].CellValue(i, j) - h_transfo;
				mFields[mNames[1]].mScalars[j][i] = mFields[mNames[1]].CellValue(i, j) + h_transfo;
			}
		}
	}
}

void Layersfield::Stabilize()
{
	//alpha: angle au repos.
	int alpha = 40;
	//epsilon: petite hauteur à faire tomber
	double epsilon = 1.;
	Scalarfield ecoulement(Box(), mFields[mNames[1]].mZMin , mFields[mNames[1]].mZMax, mNX, mNY);


	//Premiere passe : écoulement du sable dans un scalarfield temporaire
	for (unsigned j = 0; j < mNY; j++) {
		for (unsigned i = 0; i < mNX; i++) {
			std::vector<Math::Vec2u> NeighboursCoords;
			std::vector<double> NeighboursSlopes;
			std::vector<double> NeighboursDifSlope;
			double totalSlope = 0.;

			FindNeighboursFlow (i, j, NeighboursCoords, NeighboursSlopes, NeighboursDifSlope);

			const double h_bedrock = _Field(mNames[0]).CellValue(i, j);
			for (int v = 0; v < NeighboursCoords.size(); v++) {
				//calcul pente totale
				if (NeighboursSlopes[v] > std::tan(alpha)) {
					totalSlope += NeighboursSlopes[v];
				}
			}

			for (int v = 0; v < NeighboursCoords.size(); v++) {
				//repartition sable sur les voisins (pondéré)
				double slopeWeighted = NeighboursSlopes[v] / totalSlope;
				//FAUX : attention à la hauteur du sable et pas que à la hauteur de tout
				if (NeighboursSlopes[v] > std::tan(alpha)) {
					//Attention à l'arrêt de l'angle limite (?) -> on fait des petits pas
					//Au pire on dépasse d'une itération
					double deplacement = std::min(epsilon, mFields[mNames[1]].CellValue(i, j)) * slopeWeighted;///*hauteur de sable restante*/);
					mFields[mNames[1]].mScalars[j][i] -= deplacement;
					ecoulement.mScalars[NeighboursCoords[v].y][NeighboursCoords[v].x] += deplacement;
				}
			}
		}
	}

	//somme du scalarfield écoulement avec la couche de sable
	for (unsigned j = 0; j < mNY; j++) {
		for (unsigned i = 0; i < mNX; i++) {
			mFields[mNames[1]].mScalars[j][i] += ecoulement.mScalars[j][i];
		}
	}

}


void Layersfield::Save(const std::string& path)
{
	unsigned char *data = new unsigned char[mNY * mNX * 3];

	for (auto& name : mNames) {
		unsigned n = 0;
		auto& mScalar = mFields[name];
		auto& mColor = mColors[name];

		for (unsigned j = 0; j < mNY; ++j)
		{
			for (unsigned i = 0; i < mNX; ++i)
			{
				if (mScalar.CellValue(i, j) != 0)
				{
					data[n++] = static_cast<unsigned char>((mScalar.CellValue(i, j) - mScalar.mZMin) * (mColor.x * 255) / (mScalar.mZMax - mScalar.mZMin));
					data[n++] = static_cast<unsigned char>((mScalar.CellValue(i, j) - mScalar.mZMin) * (mColor.y * 255) / (mScalar.mZMax - mScalar.mZMin));
					data[n++] = static_cast<unsigned char>((mScalar.CellValue(i, j) - mScalar.mZMin) * (mColor.z * 255) / (mScalar.mZMax - mScalar.mZMin));
				}
				else
					n += 3;
			}
		}
	}
	
		stbi_write_jpg(path.c_str(), int(mNY), int(mNX), 3, data, 100);
		
		delete[] data;
}