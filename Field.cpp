#include <cfloat>
#include <cmath>
#include "Field.hpp"


double Field::HorizonSlope(const Math::Vec3d& pos, const Math::Vec2d& dir) const{
	
	const Boxd b = _Box();
	
	Math::Vec2d actPos = Math::Vec2d(pos.x, pos.y) + dir;
	int nbStep = 1;
	double pente = -DBL_MAX;
	
	while(b.IsInside(actPos)){
		double h = Height(actPos.x, actPos.y) - pos.z;
		double tmp = h / nbStep;
		
		if (tmp > pente){
			pente = tmp;
		}
		
		actPos += dir;
		nbStep += 1;
	}
	
	return pente;
	
}


bool Field::Visible(const Math::Vec3d& pos, const Math::Vec3d& point) const{
	if (pos.z < Height(pos.x, pos.y))
		return false;
	Math::Vec3d dir = point - pos;
	
	Math::Vec2d dir2D(dir.x, dir.y);
	
	double pente = dir.z / dir2D.Length();
	
	dir2D.Normalize();
	
	return pente > HorizonSlope(pos, dir2D);
}
