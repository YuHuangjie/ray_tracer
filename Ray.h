#ifndef RAY_H
#define RAY_H

#include "Point4.h"
#include "Vector4.h"
#include <iostream>

using namespace std;

class Ray
{
public:
	Ray();
	Ray(Point4, Vector4);

	Point4 GetOrigin(void) const;
	void SetOrigin(const Point4&);

	Vector4 GetDirection(void) const;
	void SetDirection(const Vector4&);

	friend ostream& operator<<(ostream&, const Ray&);

private:
	Point4 origin;
	Vector4 direction;
};


#endif