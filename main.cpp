#include "Heightfield.hpp"

int main()
{
	Boxd box;
	box.min = Math::Vec2d(0, 0);
	box.max = Math::Vec2d(10000, 10000);
	Scalarfield field("heightfield.png", box, 0, 500);

	field.ExportToObj("sfield.obj", 100, 100);

	field.Save("saved.png");
	return 0;
}
