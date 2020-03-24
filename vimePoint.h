#pragma once
class vimePoint
{
public:
	double x, y;

	vimePoint();
	vimePoint(const vimePoint& p);
	vimePoint(double x, double y);

	vimePoint& operator=(const vimePoint& p);
	bool operator==(const vimePoint& p);
};

