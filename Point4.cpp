#include "Point4.h"

const double Point4::w = 1;

Point4::Point4()
{}

Point4::Point4(double x, double y, double z)
	:x(x), y(y), z(z)
{}

Point4 Point4::operator-(const Vector4 &v) const
{
	return *this + v * (-1);
}

Point4 Point4::operator+(const Vector4 &v) const
{
	return Point4(x + v.x, y + v.y, z + v.z);
}

Vector4 Point4::operator-(const Point4 &p) const
{
	return Vector4(x - p.x, y - p.y, z - p.z);
}

ostream& operator<<(ostream &os, const Point4 &p)
{
	os << "(" << p.x << ", " << p.y << ", " << p.z << ")";
	return os;
}