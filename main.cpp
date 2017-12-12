#include "Heightfield.hpp"

int main()
{
	Boxd box;
	box.a = Math::Vec2d(0, 0);
	box.b = Math::Vec2d(1000, 1000);
	Heightfield field("hf.jpg", box, 0, 80);

	field.ExportToObj("field.obj", 500, 500);

	field.Save("saved.png");
	auto maps = field.SlopeMap();
	maps.first.Save("x_slope.png", Color::Yellow);
	maps.second.Save("y_slope.png", Color::Yellow);
	return 0;
}
