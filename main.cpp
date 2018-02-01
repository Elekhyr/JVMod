#include "Heightfield.hpp"
#include "Layersfield.hpp"

int main()
{
	Boxd box;
	box.a = Math::Vec2d(0, 0);
	box.b = Math::Vec2d(1000, 1000);
	
	Scalarfield field1("heightfield.png", box, 0, 500);

	Layersfield lf("becrock", field1);

	lf.GenerateVegetation(30, 3, 32, 32, lf.WetnessMap()).Save("trololol.png");
	/*lf.AddField("f1", field1, Math::Vec3d(0.6, 0.4, 0.1));
	lf.Thermal();
	lf.Save("lferode.png");*/
	return 0;
}
