#include "Noise.hpp"

void Noise::Noisify(Scalarfield& field)
{
	for (unsigned i = 0; i < field.mScalars.size(); ++i)
		for (unsigned j = 0; j < field.mScalars[i].size(); ++j)
		{
			const double x = i / static_cast<double>(field.mScalars[0].size()) + field.mBox.a.x;
			const double y = j / static_cast<double>(field.mScalars.size()) + field.mBox.a.y;
			field.mScalars[i][j] = At(Math::Vec2d(x, y));
		}
}