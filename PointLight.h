#ifndef POINTLIGHT_H
#define POINTLIGHT_H

#include "Point4.h"
#include "Color.h"
#include "Light.h"
#include <iostream>

using namespace std;

class PointLight : public Light
{
public:
	PointLight();
	PointLight(const Point4 &pos);

	void Illuminate(const Point4 &, Vector4 &, Color &, double &) const;

	void SetPosition(const Point4 &pos);
	Point4 GetPosition(void) const;

	friend ostream& operator<< (ostream&, const PointLight &);

private:
	Point4 position;
};




#endif