
#include "Misc.hpp"
#include "math.h"

static float DistanceBetweenPoints(float dX0, float dY0, float dX1, float dY1)
{
    return sqrt( (dX1-dX0)*(dX1-dX0) + (dY1-dY0)*(dY1-dY0));
}

bool IsTouching(const Placeable &a, const Placeable &b) {
	if (DistanceBetweenPoints(a.X,a.Y,b.X,b.Y) < b.Radius + a.Radius) {
		return true;
	}
	return false;
}