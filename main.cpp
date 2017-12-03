
#include "Scalarfield.hpp"

int main()
{
	Boxd box;
	box.min = Math::Vec2d(0, 0);
	box.max = Math::Vec2d(1280, 1280);
	Scalarfield field("heightfield.png", box, 10, 50);

	field.ExportToObj("field.obj", 500, 500);
	return 0;
}
