﻿#include "Noise.hpp"

void Noise::Noisify(Scalarfield& field, const unsigned short level)
{
	for (unsigned i = 0; i < field.mScalars.size(); ++i)
		for (unsigned j = 0; j < field.mScalars[i].size(); ++j)
		{
			const double x = i / static_cast<double>(field.mScalars[0].size()) + field.mBox.a.x;
			const double y = j / static_cast<double>(field.mScalars.size()) + field.mBox.a.y;
			
			double elevation = 0;
			double freq = 1;
			for (unsigned n = 1; n <= level; ++n)
			{
				elevation += (1 / n) * At(Math::Vec2d(freq * x, freq * y));
				freq *= 2;
			}
			field.mScalars[i][j] = elevation;
		}
}