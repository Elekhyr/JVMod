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
	field1.Save("saved.png");

	Scalarfield field(box, 0, 500, 512, 512);
	AnalyticHeightField ahf(box, 1, 5, 10);
	field.ScalarFromNoise(ahf);
	field.Save("bruit.png");
	field.ExportToObj("bruit.obj", 512, 512);
	Layersfield lf("bedrock", field1);
	lf.DrainArea().Save("drainarea.png");
	lf.Thermal();
	lf.Stabilize();
	lf.SlopeMap().Save("slopemap.png");
	lf.StreamPowerMap().Save("streampower.png");
	auto map = lf.WetnessMap();
	map.Save("wetness.png");
	lf.GenerateVegetation(50, 2, 32, 32, map).Save("veget.png");

	lf.AddField("veget", lf.GenerateVegetation(300, 10, 32, 32, map), Math::Vec3d(0, 1, 0));

	lf.Save("trololo.png");
	/*lf.AddField("f1", field1, Math::Vec3d(0.6, 0.4, 0.1));
	lf.Thermal();
	lf.Save("lferode.png");*/
	return 0;
}
