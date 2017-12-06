#include "Scalarfield.hpp"

int main()
{
	Boxd box;
	box.min = Math::Vec2d(0, 0);
	box.max = Math::Vec2d(1000, 1000);
	Scalarfield field("heightfield.png", box, 10, 11);

	field.ExportToObj("field.obj", 300, 500);

	field.Save("saved.png");
	return 0;
}
