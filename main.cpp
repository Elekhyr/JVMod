#include "Heightfield.hpp"
#include "AnalyticHeightField.hpp"
#include "Layersfield.hpp"

int main()
{
	Boxd box;
	box.a = Math::Vec2d(0, 0);
	box.b = Math::Vec2d(1000, 1000);
	
	Scalarfield sf("/Users/lois/Documents/M2/MJV/JVMod/pente.png", box, 0, 500);
	sf.Save("/Users/lois/Documents/M2/MJV/JVMod/pente2.png");
	
	Layersfield lf(512, 512, box);
	lf.AddField("bedrock", sf);
	
	Scalarfield drain = lf.DrainArea();
	
	drain.Save("/Users/lois/Documents/M2/MJV/JVMod/drainTest.png");

	return 0;
}
