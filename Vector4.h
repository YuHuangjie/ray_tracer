#ifndef VECTOR4_H
#define VECTOR4_H

#include "Point4.h"
#include <iostream>

using namespace std;

class Point4;

class Vector4
{
public:
	Vector4();
	Vector4(double, double, double);
	
	/* operators */
	Vector4 operator-(const Vector4&) const; // vector - vector = vector
	Vector4 operator+(const Vector4&) const; // vector + vector = vector
	Point4 operator+(const Point4&) const;	// vector + point = point
	
	Vector4 operator/(double mag) const;	// vector / c = vector
	Vector4 operator*(double mag) const;	// vector * c = vector

	Vector4 operator-(void) const;	

	void Normalize(void);			
	Vector4 NormalizedVec(void);
	double Norm(void) const;

	double operator*(const Vector4 &v) const;	// dot product
	Vector4 Cross(const Vector4 &v) const;		// cross product

	friend ostream& operator<<(ostream &os, const Vector4& v);
	friend Vector4 abs(const Vector4 &);

public:
	double x;
	double y;
	double z;
	static const double w;
};


#endif