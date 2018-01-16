#include "Heightfield.hpp"
#include "SimplexNoise.hpp"
#include "AnalyticHeightField.hpp"
#include "Layersfield.hpp"

int main()
{
	Boxd box;
	box.a = Math::Vec2d(0, 0);
	box.b = Math::Vec2d(1000, 1000);
	
	Scalarfield field("blank.jpg", box, 0, 500);
	AnalyticHeightField ahf = AnalyticHeightField(box, 1, 3, 3);

	field.ScalarFromNoise(ahf);
	field.Save("noisified.jpg");

	field.ExportToObj("noisified.obj", 512, 512);
	return 0;
}
