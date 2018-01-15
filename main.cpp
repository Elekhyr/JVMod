#include "Heightfield.hpp"
#include "SimplexNoise.hpp"
#include "Layersfield.hpp"

int main()
{
	Boxd box;
	box.a = Math::Vec2d(0, 0);
	box.b = Math::Vec2d(1000, 1000);
	
	Scalarfield field1("heightfield.png", box, 0, 100);
	Scalarfield field2("hf.jpg", box, 0, 100);
	
	field1.Save("wtf.jpg");
	field2.Save("wtf2.jpg");
	Layersfield lf(512, 512, box);
	lf.AddField("f1", field1, Math::Vec3d(1., 0., 1.));
	lf.AddField("f2", field2, Math::Vec3d(1., 1., 0));
	
	lf.Save("WAW.jpg", Color::Gray);

	return 0;
}
