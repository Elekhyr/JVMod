#define STB_IMAGE_IMPLEMENTATION 1
#include <fstream>
#include <iostream>
#include <array>
#include "stb_image.h"
#include "stb_image_write.h"
#include "Scalarfield.hpp"

const Boxd& Scalarfield::_Box() const
{
	return mBox;
}

double Scalarfield::Scalar(const double& x, const double& y) const
{
	// Local coordinates between [0..1]
	double u = (x - mBox.a.x) / (mScaleX);
	double v = (y - mBox.a.y) / (mScaleY);
	
	// Cell location within grid
	const unsigned row = unsigned(v * mScalars.size());
	const unsigned col = unsigned(u * mScalars[0].size());
	
	// Local coordinates within cell between [0..1]
	v = v * mScalars.size() - row;
	u = u * mScalars[0].size() - col;

	return BilinearInterpolation(row, col, u, v);
}

double Scalarfield::Scalar(unsigned i, unsigned j) const
{
	return mScalars[j][i];
}

Math::Vec3d Scalarfield::Vertice(unsigned i, unsigned j) const
{
	double x = i / (double)mScalars[0].size() + mBox.a.x;
	double y = j / (double)mScalars.size() + mBox.a.y;
	return Math::Vec3d(x, y, Scalar(i, j));
}

unsigned Scalarfield::GridXIndex(const double & x) const
{
	// Local coordinates between [0..1]
	double u = (x - mBox.a.x) / (mScaleX);

	// Cell location within gri
	const unsigned col = unsigned(u * mScalars[0].size());

	return col;
}

unsigned Scalarfield::GridYIndex(const double & y) const
{
	// Local coordinates between [0..1]
	double v = (y - mBox.a.y) / (mScaleY);

	// Cell location within grid
	const unsigned row = unsigned(v * mScalars.size());
	return row;
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
		for (double x = mBox.a.x; i < nbPointsX; x += step_x, ++i)
		{
			unsigned j = 0;
			for (double y = mBox.a.y; j < nbPointsY; y += step_y, ++j)
			{
				file << "v " << x << " " << y << " " << Scalar(x, y) << "\n";
				
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

void Scalarfield::Save(const std::string& path, const Color& color)
{
	if (color == Color::Gray)
	{
		unsigned char *data = new unsigned char[mScalars.size() * mScalars[0].size()];
		unsigned n = 0;

		for (unsigned i = 0; i < mScalars.size(); ++i)
		{
			for (unsigned j = 0; j < mScalars[i].size(); ++j)
			{
				data[n] = unsigned((mScalars[i][j] - mZMin) * 255 / (mZMax - mZMin));
				++n;
			}
		}

		stbi_write_png(path.c_str(), int(mScalars.size()), int(mScalars[0].size()), 1, data, int(mScalars.size()));

		delete[] data;
	}
	else
	{
		unsigned char *data = new unsigned char[mScalars.size() * mScalars[0].size() * 3];
		unsigned n = 0;

		for (unsigned i = 0; i < mScalars.size(); ++i)
		{
			for (unsigned j = 0; j < mScalars[i].size(); ++j)
			{
				unsigned r = 0;
				unsigned g = 0;
				unsigned b = 0;

				unsigned result = unsigned((mScalars[i][j] - mZMin) * 255 / (mZMax - mZMin));
				
				if (color == Color::Red || color == Color::Purple || color == Color::Yellow)
					r = result;
				data[n++] = r;
				
				if (color == Color::Green || color == Color::Yellow || color == Color::Cyan)
					g = result;
				data[n++] = g;

				if (color == Color::Blue || color == Color::Purple || color == Color::Cyan)
					b = result;
				data[n++] = b;
			}
		}

		stbi_write_jpg(path.c_str(), int(mScalars.size()), int(mScalars[0].size()), 3, data, 100);

		delete[] data;
	}
}

Scalarfield::Scalarfield(const std::string& imagePath, const Boxd& boudingBox, const double zmin, const double zmax)
{
	mZMin = zmin;
	mZMax = zmax;

	mBox = boudingBox;
	
	int img_width, img_height, nb_channels;
	unsigned char* image_data = stbi_load(imagePath.c_str(), &img_width, &img_height, &nb_channels, STBI_rgb);
	if (image_data != nullptr)
	{
		mScalars = std::vector<std::vector<double>>(img_height, std::vector<double>(img_width));

		const unsigned size = img_width * img_height * nb_channels;
		unsigned row = 0;
		unsigned col = 0;
		for (unsigned n = 0; n < size; n += nb_channels, ++col)
		{
			if (col == img_width)
			{
				col = 0;
				++row;
			}
			for (int c = 0; c < nb_channels; ++c)
			{
				if (image_data[n + c] != 0)
				{
					mScalars[row][col] = zmin + (zmax - zmin) * image_data[n + c] / 255;
					break;
				}
			}
		}
		mScaleX = mBox.b.x - mBox.a.x;
		mScaleY = mBox.b.y - mBox.a.y;

		mNX = mScalars[0].size();
		mNY = mScalars.size();
		
		stbi_image_free(image_data);
	}
}

double Scalarfield::BilinearInterpolation(const unsigned row, const unsigned col, const double& u, const double& v) const 
{
	double result = Scalar(row, col);
	// Interpolation with the bottom and left cells
	if (u + v < 1)
	{
		if (row + 1 < mScalars.size())
		{
			result -= v * Scalar(row, col);
			result += v * Scalar(row + 1, col);
		}

		if (col + 1 < mScalars[0].size())
		{
			result -= u * Scalar(row, col);
			result += u * Scalar(row, col + 1);
		}
	}
	else // interpolation with the top and right cells
	{
		if (int(row) - 1 >= 0)
		{
			result -= v * Scalar(row, col);
			result += v * Scalar(row - 1, col);
		}

		if (int(col) - 1 >= 0)
		{
			result -= u * Scalar(row, col);
			result += u * Scalar(row, col - 1);
		}
	}

	return result;
}
