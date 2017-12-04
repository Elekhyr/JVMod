#include "Heightfield.hpp"
#include <fstream>
#include <iostream>
#include <array>

double Heightfield::Height(const double& x, const double& y) const
{
	return Scalar(x, y);
}

Math::Vec3d Heightfield::Normal(const double x, const double y) const
{
	const double epsilon_x = 1 / mScaleX;
	const double epsilon_y = 1 / mScaleY;

	Math::Vec3d a(x - epsilon_x, y, Scalar(x - epsilon_x, y));
	Math::Vec3d b(x + epsilon_x, y, Scalar(x + epsilon_x, y));
	
	Math::Vec3d c(x, y - epsilon_y, Scalar(x, y - epsilon_y));
	Math::Vec3d d(x, y + epsilon_y, Scalar(x, y + epsilon_y));

	return (b-a) * (d-c);
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
		for (double x = mBox.min.x; i < nbPointsX; x += step_x, ++i)
		{
			unsigned j = 0;
			for (double y = mBox.min.y; j < nbPointsY; y += step_y, ++j)
			{
				double z = Scalar(x, y);
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
			file << "vn " << n[0] << " " << n[1] << " " << n[2] << " \n";
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
