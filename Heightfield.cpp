#define STB_IMAGE_IMPLEMENTATION 1
#include "stb_image.h"

#include "Heightfield.hpp"

Heightfield::Heightfield(const std::string& imagePath, const Boxd& boudingBox, const double zmin, const double zmax,
	const std::vector<std::vector<double>>& alphaMap, const Noise& noise): Scalarfield()
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
		mScalars[row][col] = zmin + (zmax - zmin) * image_data[n] / 255;
	}
	mScaleX = mBox.max.x - mBox.min.x;
	mScaleY = mBox.max.y - mBox.min.y;

	stbi_image_free(image_data);
}
