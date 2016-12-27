#include "Vector4.h"
#include <cmath>

const double Vector4::w = 0;

Vector4::Vector4()
{}

Vector4::Vector4(double x, double y, double z)
	: x(x), y(y), z(z)
{}

Vector4 Vector4::operator-(const Vector4 &v) const
{
	return Vector4(x - v.x, y - v.y, z - v.z);
}

Vector4 Vector4::operator+(const Vector4 &v) const
{
	return Vector4(x + v.x, y + v.y, z + v.z);
}

Point4 Vector4::operator+(const Point4 &p) const
{
	return Point4(x + p.x, y + p.y, z + p.z);
}

Vector4 Vector4::operator/(double mag) const
{
	double m = 1.0 / mag;
	return (*this) * m;
}

Vector4 Vector4::operator*(double mag) const
{
	return Vector4(x * mag, y * mag, z * mag);
}

Vector4 Vector4::operator-(void) const
{
	return *this * (-1);
}

void Vector4::Normalize(void)
{
	(*this) = (*this) / this->Norm();
}

Vector4 Vector4::NormalizedVec(void)
{
	return (*this) / this->Norm();
}

double Vector4::Norm(void) const
{
	return std::sqrt(x*x + y*y + z*z);
}

double Vector4::operator*(const Vector4& v) const
{
	return (x*v.x + y*v.y + z*v.z);
}

Vector4 Vector4::Cross(const Vector4& v) const
{
	return Vector4((y*v.z - z*v.y), (z*v.x - x*v.z), (x*v.y - y*v.x));
}

ostream& operator<<(ostream &os, const Vector4 &v)
{
	os << "<" << v.x << ", " << v.y << ", " << v.z << ">";
	return os;
}

Vector4 abs(const Vector4 &v)
{
	return Vector4(abs(v.x), abs(v.y), abs(v.z));
}