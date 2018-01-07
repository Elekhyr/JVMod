#include <fstream>
#include <iostream>
#include <array>
#include "Heightfield.hpp"

double Heightfield::Height(const double& x, const double& y) const
{
	return Scalar(x, y);
}

double Heightfield::HeightCell(unsigned i, unsigned j) const
{
	return Scalar(i, j);
}

double Heightfield::Height(const Math::Vec2d& pos) const
{
	return Scalar(pos.x, pos.y);
}

unsigned Heightfield::_SizeX() const
{
	return mNX;
}

unsigned Heightfield::_SizeY() const
{
	return mNY;
}

double Heightfield::_ScaleX() const
{
	return mBox.b.x - mBox.a.x;
}

double Heightfield::_ScaleY() const
{
	return mBox.b.y - mBox.a.y;
}

void Heightfield::ExportToObj(const std::string & path, unsigned nbPointsX, unsigned nbPointsY) const
{
	std::vector<std::array<unsigned, 3>> faces;
	faces.reserve(nbPointsX * nbPointsY * 2);


	std::vector<std::array<double, 3>> normals;
	normals.reserve(nbPointsX * nbPointsY);
	
	unsigned n = 0;
	unsigned i = 0;

	std::ofstream file;
	file.exceptions(std::ofstream::badbit | std::ofstream::failbit);

	try
	{
		file.open(path);
	}
	catch (std::ofstream::failure& e)
	{
		std::cerr << e.what() << std::endl;
	}

	if (file.is_open())
	{
		file << "g heightfield\n\n";

		const double step_x = mScaleX / double(nbPointsX);
		const double step_y = mScaleY / double(nbPointsY);
		for (double x = mBox.a.x; i < nbPointsX; x += step_x, ++i)
		{
			unsigned j = 0;
			for (double y = mBox.a.y; j < nbPointsY; y += step_y, ++j)
			{
				double z = Scalar(i, j);
				file << "v " << x << " " << y << " " << z << "\n";
				auto normal = Normal(x, y);
				normals.push_back({normal.x, normal.y, normal.z});

				if (i < nbPointsX - 1 && j < nbPointsY - 1)
				{
					faces.push_back({ n + j + 2,
						n + j + 1,
						n + j + nbPointsX + 1 });
					faces.push_back({ n + j + nbPointsX + 1,
						n + j + nbPointsX + 2,
						n + j + 2 });
				}
			}
			n += j;
		}

		file << "\n\n";
		for (auto& n : normals)
		{
			//file << "vn " << n[0] << " " << n[1] << " " << n[2] << " \n";
		}


		file << "\n\n";
		for (auto& f : faces)
		{
			file << "f " 
				<< f[0] << "//" << f[0] << " " 
				<< f[1] << "//" << f[1] << " " 
				<< f[2] << "//" << f[2] << " \n";
		}

		file.close();
	}
}

const Boxd& Heightfield::Box() const
{
	return _Box();
}
