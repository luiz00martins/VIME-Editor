#include "vimeWall.h"

vimeWall::vimeWall() {
	p0.x = 0;
	p0.y = 0;
	p1.x = 0;
	p1.y = 0;
	texturePath = "";
}

vimeWall::vimeWall(const vimeWall& w) {
	p0 = w.p0;
	p1 = w.p1;
	texturePath = "";
}

vimeWall::vimeWall(double xx0, double yy0, double xx1, double yy1) {
	p0.x = xx0;
	p0.y = yy0;
	p1.x = xx1;
	p1.y = yy1;
	texturePath = "";
}

vimeWall::vimeWall(vimePoint pp0, vimePoint pp1) {
	p0 = pp0;
	p1 = pp1;
	texturePath = "";
}

void vimeWall::setTexturePath(std::string str) {
	texturePath = str;
}

vimeWall& vimeWall::operator=(const vimeWall& w) {
	p0 = w.p0;
	p1 = w.p1;
	texturePath = w.texturePath;

	return *this;
}

bool vimeWall::operator==(const vimeWall& w) {
	return (p0 == w.p0 && p1 == w.p1);
}
