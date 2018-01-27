#include "Heightfield.hpp"
#include "Layersfield.hpp"

int main()
{
	Boxd box;
	box.a = Math::Vec2d(0, 0);
	box.b = Math::Vec2d(1000, 1000);
	
	Scalarfield field1("heightfield.png", box, 0, 500);
	Scalarfield field2("hf.jpg", box, 0, 500);

	Layersfield lf(512, 512, box);
	lf.AddField("f1", field1, Math::Vec3d(0.6, 0.4, 0.1));
	lf.AddField("f2", field2, Math::Vec3d(0.4, 0.8, 0.8));
	lf.Thermal();
	lf.Save("lferode.png");
	return 0;
}
