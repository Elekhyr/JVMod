#include "Heightfield.hpp"
#include "SimplexNoise.hpp"
int main()
{
	Boxd box;
	box.a = Math::Vec2d(0, 0);
	box.b = Math::Vec2d(1000, 1000);
	
	Scalarfield field("blank.jpg", box, -1, 1);
	SimplexNoise noise;

	noise.Noisify(field, 1);
	field.Save("noisified.jpg");

	return 0;
}
