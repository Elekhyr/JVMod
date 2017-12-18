#include "Heightfield.hpp"

int main()
{
	Boxd box;
	box.a = Math::Vec2d(0, 0);
	box.b = Math::Vec2d(1000, 1000);
	
	Heightfield hf("heightfield.png", box, 10, 500);
	hf.ExportToObj("test.obj", 200, 200);
	return 0;
}
