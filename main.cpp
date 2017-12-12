#include "Heightfield.hpp"

int main()
{
	Boxd box;
	box.a = Math::Vec2d(0, 0);
	box.b = Math::Vec2d(500, 500);
	Scalarfield field("sarek45km.png", box, 0, 100);

	field.ExportToObj("field.obj", 50, 50);

	Heightfield hfield("sarek45km.png", box, 0, 100);
	hfield.Visible(Math::Vec3d(0,0,0), Math::Vec3d(500, 500, 1200));

	hfield.ExportToObj("hfield.obj", 500, 500);

	field.Save("saved.png");
	return 0;
}
