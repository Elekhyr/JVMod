#include "Heightfield.hpp"
#include "SimplexNoise.hpp"
#include "AnalyticHeightField.hpp"
#include "Layersfield.hpp"

int main()
{

	Boxd box;
	box.a = Math::Vec2d(0, 0);
	box.b = Math::Vec2d(512, 512);
	
	Scalarfield field1("heightfield.png", box, 0, 200);
	field1.ExportToObj("camarcheopupas.obj", 512, 512);
	
	Layersfield lf("becrock", field1);

	lf.AddField("veget", lf.GenerateVegetation(50, 2, 512, 512, lf.WetnessMap()), Math::Vec3d(0, 1, 0));

	lf.Save("trololo.png");
	/*lf.AddField("f1", field1, Math::Vec3d(0.6, 0.4, 0.1));
	lf.Thermal();
	lf.Save("lferode.png");*/
	return 0;
}
