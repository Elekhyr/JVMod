#include "Layersfield.hpp"
#define STB_IMAGE_WRITE_STATIC
#include "stb_image_write.h"
#include "Logger.h"

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
#ifdef GIMME_LOG_PLZ
	const std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
#endif
	//verification du nombre de layers et ajout du sand si 1 seul layer
	if (mFields.size() < 2){
		AddEmptyField("sand");
	}

	double delta_h = 0.;
	//hauteur à partir de laquelle on commence à transformer
	const double delta_h_0 = 0.1;

	//coefficient de transformation
	const double k = 0.1;

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
#ifdef GIMME_LOG_PLZ
	std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();

	olog(Info) << __FUNCTION__ << " has been executed in : " << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
#endif
}

bool myCompareVec(std::pair<double, Math::Vec2i>& a, std::pair<double, Math::Vec2i>& b){
	return a.first < b.first;
}

void Layersfield::Stabilize()
{
#ifdef GIMME_LOG_PLZ
	const std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
#endif
	//alpha: angle au repos.
	int alpha = 40;
	//epsilon: petite hauteur à faire tomber
	double epsilon = 0.1;

	Scalarfield ecoulement(Box(), mFields[mNames[1]].mZMin , mFields[mNames[1]].mZMax, mNX, mNY);
	std::vector<std::pair<double, Math::Vec2i>> sorted_heights(_SizeX() * _SizeY());
	
	for (unsigned i = 0; i < _SizeX(); i++) {
		for (unsigned j = 0; j < _SizeY(); j++) {
			sorted_heights[i * _SizeY() + j] = std::make_pair(HeightCell(i, j), Math::Vec2i(i,j));
		}
	}

	std::sort(sorted_heights.rbegin(), sorted_heights.rend(), myCompareVec);

	//Premiere passe : écoulement du sable dans un scalarfield temporaire
	for (auto& p : sorted_heights) {
		unsigned i = unsigned(p.second.x); //coordonnées de la case en cours de traitement
		unsigned j = unsigned(p.second.y);
		std::vector<Math::Vec2u> neighbour_coords;
		std::vector<double> neighbour_slopes;
		std::vector<double> neighbour_heights_diff;
		FindNeighboursFlow(i, j, neighbour_coords, neighbour_slopes, neighbour_heights_diff);
		
		double totalSlope = 0.;

		for (auto& val : neighbour_slopes) {
			//calcul pente totale
			if (val > std::tan(alpha)) {
				totalSlope += val;
			}
		}

		for (int v = 0; v < neighbour_coords.size(); v++) {
			//repartition sable sur les voisins (pondérée)
			if (neighbour_slopes[v] > std::tan(alpha)) {
				double slopeWeighted = neighbour_slopes[v] / totalSlope;
				//Attention à l'arrêt de l'angle limite (?) -> on fait des petits pas
				//Au pire on dépasse d'une itération
				double deplacement = std::min(epsilon, mFields[mNames[1]].CellValue(i, j)) * slopeWeighted;///*hauteur de sable restante*/);
				mFields[mNames[1]].mScalars[j][i] -= deplacement;
				ecoulement.mScalars[neighbour_coords[v].y][neighbour_coords[v].x] += deplacement;
			}
		}
	}

	//somme du scalarfield écoulement avec la couche de sable
	mFields[mNames[1]] += ecoulement;
	// for (unsigned j = 0; j < mNY; j++) {
	// 	for (unsigned i = 0; i < mNX; i++) {
	// 		mFields[mNames[1]][j][i] += ecoulement.CellValue(i,j);
	// 	}
	// }
#ifdef GIMME_LOG_PLZ
	std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();

	olog(Info) << __FUNCTION__ << " has been executed in : " << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
#endif
}

void Layersfield::SaveTotal(const std::string& path)
{
	// Scalarfield total(Box(), 0, 400, mNX, mNY);
	Scalarfield total(mFields[mNames[0]]);
	for (int i = 1; i < mNames.size(); i++) {
		total += _Field(mNames[i]);
	}
	// for (int j = 0; j < mNY; j++) {
	// 	for (int i = 0; i < mNX; i++) {
	// 		total.mScalars[j][i] = HeightCell(i,j);
	// 	}
	// }
	total.Save(path);
}

void Layersfield::Save(const std::string& path)
{
	unsigned char *data = new unsigned char[mNY * mNX * 3];

	for (auto& name : mNames) {
		unsigned n = 0;
		auto& mScalar = mFields[name];
		auto& mColor = mColors[name];

		for (int j = int(mNY)-1; j >= 0; --j)
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

void Layersfield::SaveFlatColor(const std::string& path)
{
	unsigned char *data = new unsigned char[mNY * mNX * 3];

	for (auto& name : mNames) {
		unsigned n = 0;
		auto& mScalar = mFields[name];
		auto& mColor = mColors[name];

		for (int j = int(mNY)-1; j >= 0; --j)
		{
			for (unsigned i = 0; i < mNX; ++i)
			{
				if (mScalar.CellValue(i, j) >= 0.01)
				{
					data[n++] = static_cast<unsigned char>(mColor.x * 255);
					data[n++] = static_cast<unsigned char>(mColor.y * 255);
					data[n++] = static_cast<unsigned char>(mColor.z * 255);
				}
				else
					n += 3;
			}
		}
	}
	
		stbi_write_jpg(path.c_str(), int(mNY), int(mNX), 3, data, 100);
		
		delete[] data;
}
