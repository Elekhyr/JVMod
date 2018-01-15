#include "AnalyticHeightField.hpp"
#include <random>

AnalyticHeightField::AnalyticHeightField():mScaleX(0), mScaleY(0), mAmplitude(0), mLambda(0), mLevel(0)
{
}

AnalyticHeightField::AnalyticHeightField(const Boxd& boudingBox, const double amplitude, const double lambda, const unsigned level)
{
	mBox = boudingBox;
	mScaleX = mBox.b.x - mBox.a.x;
	mScaleY = mBox.b.y - mBox.a.y;

	mAmplitude = amplitude;
	mLambda = lambda;
	mLevel = level;
}

double AnalyticHeightField::SimplexNoiseAt(const Math::Vec2<double> v) const
{
	SimplexNoise noise = SimplexNoise();
	double elevation = 0;
	for (unsigned n = 0; n < mLevel; ++n)
	{
		double pow = std::pow(2, n);
		double a = mAmplitude / pow;
		double l = mLambda / pow;

		elevation += a * (noise.At(Math::Vec2d(v.x / l, v.y / l)) /*+ sigma*/);
	}
	return elevation;
}

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