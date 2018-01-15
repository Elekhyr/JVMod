#include "Noise.hpp"
#include <random>
// void Noise::Noisify(Scalarfield& field, const unsigned short level, const unsigned amplitude, const unsigned length)
// {

// 	for (unsigned i = 0; i < field.mScalars.size(); ++i)
// 		for (unsigned j = 0; j < field.mScalars[i].size(); ++j)
// 		{
// 			const double x = i / static_cast<double>(field.mScalars[0].size()) + field.mBox.a.x;
// 			const double y = j / static_cast<double>(field.mScalars.size()) + field.mBox.a.y;
			
// 			double elevation = 0;
// 			for (unsigned n = 0; n < level; ++n)
// 			{
// 				double pow = std::pow(2, n);
// 				double a = amplitude / pow;
// 				double l = length / pow;

// 				elevation += a * (At(Math::Vec2d(x / l, 1.5 * y / l)) /*+ sigma*/);
// 			}
// 			field.mScalars[i][j] = elevation;
// 		}
// }