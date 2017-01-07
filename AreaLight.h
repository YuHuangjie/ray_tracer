#ifndef AREALIGHT_H
#define AREALIGHT_H

#include "Point4.h"
#include "Color.h"
#include "Light.h"
#include <iostream>
#include <memory>

using namespace std;

class AreaLight : public Light
{
public:
	AreaLight();
	// note: It will be tedious to give a parameter for right direction.
	//       Instead, we will set a default one.
	AreaLight(const Point4 &cen, const Vector4 &norm, const double &w, const double &h);

	void Illuminate(const Point4 &, vector<Vector4> &, vector<Color> &, vector<double> &) const;

	//void SetCenter(const Point4 &cen);
	//void SetNormal(const Vector4 &nor);
	//void SetRightDir(const Vector4 &right);
	//void SetWH(const double &w, const double &h);
	void SetSample(const uint16_t nx, const uint16_t ny);


	friend ostream& operator<< (ostream&, const AreaLight &);


private:
	Point4 area_center;
	Vector4 area_normal;
	Vector4 area_rightdir;
	double area_width;
	double area_height;

	uint16_t wSample;
	uint16_t hSample;
	unique_ptr<Point4[]> samplePositions;

};




#endif