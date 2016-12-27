#ifndef BBOX_H
#define BBOX_H

#include "Triangle.h"
#include <memory>
#include <vector>
#include <iostream>

using namespace std;

enum Axis
{
	X_Axis = 0,
	Y_Axis = 1,
	Z_Axis = 2
};

class BBox
{
public:
	BBox();
	BBox(const BBox &);
	
	static BBox FromTriangles(const vector< shared_ptr<Triangle> > &);
	
	void SetX(double min, double max);
	void SetY(double min, double max);
	void SetZ(double min, double max);
	void GetX(double &, double &) const;
	void GetY(double &, double &) const;
	void GetZ(double &, double &) const;

	Axis GetLongestAxis(void) const;
	bool Intersect(const Ray &, double &dist) const;
	//bool Intersect(const Ray &, Point4 &) const;
	//bool Intersect(const Ray &) const;
	bool IsValid(void) const;

	friend ostream& operator<<(ostream&, const BBox &);

private:
	double x_min, x_max;
	double y_min, y_max;
	double z_min, z_max;

	static BBox FromTriangle(shared_ptr<Triangle> );
	static BBox Merge(const BBox &, const BBox &);
};


#endif