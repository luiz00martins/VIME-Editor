#pragma once

#include <string>
#include "vimePoint.h"

class vimeWall
{
public:
	vimePoint p0, p1;
	std::string texturePath;


public:
	vimeWall();
	vimeWall(const vimeWall& w);
	vimeWall(double x0, double y0, double x1, double y1);
	vimeWall(vimePoint p0, vimePoint p1);

	void setTexturePath(std::string str);

	vimeWall& operator=(const vimeWall& w);
	bool operator==(const vimeWall& w);
		
};

