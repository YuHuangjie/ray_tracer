#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "Primitive.h"
#include "Intersection.h"
#include "Point4.h"
#include "Vector4.h"
#include <iostream>

using namespace std;

class Triangle
	: public Primitive
	, public enable_shared_from_this<Triangle>
{
public:
	Triangle();
	Triangle(const Point4 &, const Point4 &, const Point4 &);

	Intersection Intersect(const Ray &ray, double &dist) const;

	void SetVertices(const Point4 &, const Point4 &, const Point4 &);
	void GetVertices(Point4 &, Point4 &, Point4 &);

	void SetNormal(const Vector4 &);
	void GetNormal(Vector4 &);

	Point4 GetMidPoint(void) const;

	friend ostream& operator<<(ostream &, const Triangle &);

private:
	Point4 v1;
	Point4 v2;
	Point4 v3;
	Vector4 normal;
};


#endif