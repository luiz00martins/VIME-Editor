#include "vimePoint.h"

vimePoint::vimePoint() {
	x = 0;
	y = 0;
}

vimePoint::vimePoint(double xx, double yy) {
	x = xx;
	y = yy;
}

vimePoint::vimePoint(const vimePoint& p) {
	x = p.x;
	y = p.y;
}

vimePoint& vimePoint::operator=(const vimePoint& p)
{
	x = p.x;
	y = p.y;

	return *this;
}

bool vimePoint::operator==(const vimePoint& p) {
	return (x == p.x && y == p.y);
}
