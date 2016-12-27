#ifndef INFINITEPLANE_H
#define INFINITEPLANE_H

#include "Primitive.h"
#include "Point4.h"
#include "Vector4.h"
#include <iostream>

using namespace std;

class InfinitePlane 
	: public Primitive
	, public enable_shared_from_this<InfinitePlane>
{
public:
	InfinitePlane();
	InfinitePlane(const Point4 &p, const Vector4& normal);

	Intersection Intersect(const Ray &incident, double &dist) const;

	Point4 GetFixPoint(void) const;
	Vector4 GetNormal(void) const;

	friend ostream& operator<<(ostream& os, const InfinitePlane &plane);

protected:
	Point4 fixPoint;		// one point on the plane
	Vector4 normal;	// normal of plane
};


#endif