#ifndef INTERSECTION_H
#define INTERSECTION_H

#include "Point4.h"
#include "Vector4.h"
#include "Primitive.h"
#include <iostream>
#include <memory>

using namespace std;

class Primitive;

class Intersection
{
public:
	Intersection();
	Intersection(const Point4&, const Vector4 &, const shared_ptr<const Primitive> &);

	void SetPosition(const Point4 &pos);
	Point4 GetPosition(void) const;

	void SetNormal(const Vector4 &normal);
	Vector4 GetNormal(void) const;

	void SetPrimitive(const shared_ptr<const Primitive> &);
	shared_ptr<const Primitive> GetPrimitive(void) const;

	friend ostream& operator<< (ostream& os, const Intersection&);

private:
	Point4 position;
	Vector4 normal;

	shared_ptr<const Primitive> object;
};


#endif