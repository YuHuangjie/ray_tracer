#ifndef POINT4_H
#define POINT4_H

#include "Vector4.h"
#include <iostream>

using namespace std;

class Vector4;

class Point4
{
public:
	Point4();
	Point4(double, double, double);
	
	/* operators */
	Point4 operator-(const Vector4 &) const;	// point - vector = point
	Point4 operator+(const Vector4 &) const;	// point + vector = point
	Vector4 operator-(const Point4 &) const;	// point - point = vector

	friend ostream& operator<<(ostream &os, const Point4 &);

public:
	double x;
	double y;
	double z;
	static const double w;
};


#endif