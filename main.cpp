#include "Heightfield.hpp"
#include "SimplexNoise.hpp"
#include "AnalyticHeightField.hpp"
int main()
{
	Boxd box;
	box.a = Math::Vec2d(0, 0);
	box.b = Math::Vec2d(1000, 1000);
	
	Scalarfield field("blank.jpg", box, -1, 1);
	AnalyticHeightField ahf = AnalyticHeightField(box, 1, 3, 7);

	field.ScalarFromNoise(ahf);
	field.Save("noisified.jpg");

	return 0;
}
