#include <fstream>
#include <iostream>
#include <array>
#include "Heightfield.hpp"

double Heightfield::Height(const double& x, const double& y) const
{
	return Scalar(x, y);
}

double Heightfield::Height(const Math::Vec2d& pos) const
{
	return Scalar(pos.x, pos.y);
}

const Boxd & Heightfield::_Box() const
{
	return mBox;
}

Math::Vec2d Heightfield::Slope(const int i, const int j) const
{
	Math::Vec2d n;
	
	if (i + 1 < mScalars[0].size() && i - 1 >= 0)
	{
		if (i - 1 >= 0)
		{
			n.x = (mScalars[i + 1][j] - mScalars[i - 1][j]) / (2 * mScaleX / mScalars[0].size());
		}
		else
		{
			n.x = (mScalars[i + 1][j] - mScalars[i][j]) / (mScaleX / mScalars[0].size());
		}

		if (j + 1 < mScalars.size())
		{
			if (j - 1 >= 0)
				n.y = (mScalars[i][j + 1] - mScalars[i][j - 1]) / (2 * mScaleX / mScalars.size());
			else
				n.y = (mScalars[i][j + 1] - mScalars[i][j]) / (mScaleX / mScalars.size());
		}
		else {
			if (j - 1 >= 0)
				n.y = (mScalars[i][j] - mScalars[i][j - 1]) / (mScaleX / mScalars.size());
			else
				n.y = mScalars[i][j];
		}
	}
	else
	{
		if (i - 1 >= 0)
		{
			n.x = (mScalars[i][j] - mScalars[i - 1][j]) / (mScaleX / mScalars[0].size());
		}
		else
		{
			n.x = mScalars[i][j];
		}

		if (j + 1 < mScalars.size())
		{
			if (j - 1 >= 0)
				n.y = (mScalars[i][j + 1] - mScalars[i][j - 1]) / (2 * mScaleX / mScalars.size());
			else
				n.y = (mScalars[i][j + 1] - mScalars[i][j]) / (mScaleX / mScalars.size());
		}
		else {
			if (j - 1 >= 0)
				n.y = (mScalars[i][j] - mScalars[i][j - 1]) / (mScaleX / mScalars.size());
			else
				n.y = mScalars[i][j];
		}
	}

	return n;
}

std::pair<Scalarfield, Scalarfield> Heightfield::SlopeMap() const
{
	Scalarfield x_field;
	Scalarfield y_field;

	x_field.mBox = mBox;
	x_field.mScaleX = mScaleX;
	x_field.mScaleY = mScaleY;


	y_field.mBox = mBox;
	y_field.mScaleX = mScaleX;
	y_field.mScaleY = mScaleY;

	x_field.mScalars = std::vector<std::vector<double>>(mScalars.size(), std::vector<double>(mScalars[0].size()));
	y_field.mScalars = std::vector<std::vector<double>>(mScalars.size(), std::vector<double>(mScalars[0].size()));

	double min_x = 0;
	double min_y = 0;
	double max_x = 0;
	double max_y = 0;

	for (unsigned i = 0; i < mScalars.size(); ++i)
	{
		for (unsigned j = 0; j < mScalars[i].size(); ++j)
		{
			auto slope = Slope(i, j);
			x_field.mScalars[i][j] = slope.x;
			y_field.mScalars[i][j] = slope.y;

			min_x = std::min(min_x, slope.x);
			min_y = std::min(min_y, slope.y);
			max_x = std::max(max_x, slope.x);
			max_y = std::max(max_y, slope.y);
		}
	}

	x_field.mZMax = max_x;
	x_field.mZMin = min_x;

	y_field.mZMax = max_y;
	y_field.mZMin = min_y;
	return std::move(std::make_pair(std::move(x_field), std::move(y_field)));
}

double Heightfield::HorizonSlope(const Math::Vec3d & pos, const Math::Vec2d & dir) const
{
	return 0.0;
}

bool Heightfield::Visible(const Math::Vec3d & pos, const Math::Vec3d & point) const
{
	return false;
}

Math::Vec3d Heightfield::Normal(const double x, const double y) const
{
	const double epsilon_x = 1 / mScaleX;
	const double epsilon_y = 1 / mScaleY;

	Math::Vec3d a(x - epsilon_x, y, Scalar(x - epsilon_x < 0 ? x : x - epsilon_x, y));
	Math::Vec3d b(x + epsilon_x, y, Scalar(x + epsilon_x, y));
	
	Math::Vec3d c(x, y - epsilon_y, Scalar(x, y - epsilon_y < 0 ? y : y - epsilon_y));
	Math::Vec3d d(x, y + epsilon_y, Scalar(x, y + epsilon_y));

	auto n = ((b - a) * (d - c));
	n.Normalize();
	return n;
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
