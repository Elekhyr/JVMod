
#include "Scalarfield.hpp"

int main()
{
	Boxd box;
	box.min = Math::Vec2d(0, 0);
	box.max = Math::Vec2d(1000, 1000);
	Scalarfield field("heightfield.png", box, 10, 12);

	field.ExportToObj("field.obj", 300, 300);

	field.Save("saved.png");
	return 0;
}
