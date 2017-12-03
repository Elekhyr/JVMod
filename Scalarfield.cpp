#define STB_IMAGE_IMPLEMENTATION 1
#include "stb_image.h"
#include "Scalarfield.hpp"
#include <fstream>
#include <iostream>
#include <array>

double Scalarfield::GridScalar(const int i, const int j) const
{
	return mScalars[i][j];
}

double Scalarfield::Scalar(const double& x, const double& y) const
{
	// Local coordinates between [0..1]
	double u = (x - mBox.min.x) / (mScaleX);
	double v = (y - mBox.min.y) / (mScaleY);

	// Cell location within grid
	const unsigned row = unsigned(v * mScalars.size());
	const unsigned col = unsigned(u * mScalars[0].size());

	// Local coordinates within cell between [0..1]
	v = v * mScalars.size() - row;
	u = u * mScalars.size() - col;

	return BilinearInterpolation(row, col, u, v);
}

void Scalarfield::ExportToObj(const std::string& path, const unsigned nbPointsX, const unsigned nbPointsY) const
{
	std::vector<std::array<unsigned, 3>> faces;
	faces.reserve(nbPointsX * nbPointsY * 2);

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
				file << "v " << x/ mBox.max.x << " " << y/ mBox.max.y << " " << Scalar(x, y)/mMax << "\n";
				
				if (i < nbPointsX - 1 && j < nbPointsY - 1)
				{
					faces.push_back({ n + j + 2,
						n + j + 1, 
						n + j + nbPointsX + 1});
					faces.push_back({ n + j + nbPointsX + 1, 
						n + j + nbPointsX + 2, 
						n + j + 2 });
				}
			}
			n += j;
		}

		file << "\n\n";
		for (auto& f : faces)
		{
			file << "f " << f[0] << " " << f[1] << " " << f[2] << " \n";
		}

		file.close();
	}
}

Scalarfield::Scalarfield(const std::string& imagePath, const Boxd& boudingBox, const double zmin, const double zmax)
{
	mMin = zmin;
	mMax = zmax;

	mBox = boudingBox;
	
	int img_width, img_height, nb_channels;
	unsigned char* image_data = stbi_load(imagePath.c_str(), &img_width, &img_height, &nb_channels, STBI_grey);
	
	mScalars = std::vector<std::vector<double>>(img_height, std::vector<double>(img_width));

	const unsigned size = img_width * img_height;
	unsigned row = 0;
	unsigned col = 0;
	for (unsigned n = 0; n < size; ++n, ++col)
	{
		if (col == img_width)
		{
			col = 0;
			++row;
		}
		mScalars[row][col] = zmin + (zmax-zmin) * image_data[n] / 255;
	}
	
	mScaleX = mBox.max.x - mBox.min.x;
	mScaleY = mBox.max.y - mBox.min.y;
}

double Scalarfield::BilinearInterpolation(const unsigned row, const unsigned col, const double& u, const double& v) const 
{
	double result = GridScalar(row, col);
	// Interpolation with the bottom and left cells
	if (u + v < 1)
	{
		if (row + 1 < mScalars.size())
		{
			result -= v * GridScalar(row, col);
			result += v * GridScalar(row + 1, col);
		}

		if (col + 1 < mScalars[0].size())
		{
			result -= u * GridScalar(row, col);
			result += u * GridScalar(row, col + 1);
		}
	}
	else // interpolation with the top and right cells
	{
		if (int(row) - 1 >= 0)
		{
			result -= v * GridScalar(row, col);
			result += v * GridScalar(row - 1, col);
		}

		if (int(col) - 1 >= 0)
		{
			result -= u * GridScalar(row, col);
			result += u * GridScalar(row, col - 1);
		}
	}

	return result;
}
