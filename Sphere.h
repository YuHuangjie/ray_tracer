#ifndef SPHERE_H
#define SPHERE_H

#include "Primitive.h"
#include "Point4.h"
#include "Vector4.h"
#include <iostream>

using namespace std;

class Sphere 
	: public Primitive
	, public enable_shared_from_this<Sphere>
{
public:
	Sphere();
	Sphere(const Point4 &, double);

	Intersection Intersect(const Ray &ray, double &dist) const;

	Point4 GetCenter(void) const;
	double GetRadius(void) const;

	friend ostream& operator<<(ostream &, const Sphere &);

protected:
	Point4 center;
	double radius;
};


#endif