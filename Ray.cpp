#include "Ray.h"

Ray::Ray()
	: origin(0, 0, 0), direction(0, 0, 1)
{}

Ray::Ray(Point4 p, Vector4 v)
	: origin(p)
{
	direction = v.NormalizedVec();
}

Point4 Ray::GetOrigin(void) const
{
	return origin;
}

void Ray::SetOrigin(const Point4 &p)
{
	origin = p;
}

Vector4 Ray::GetDirection(void) const
{
	return direction;
}

void Ray::SetDirection(const Vector4 &v)
{
	direction = Vector4(v).NormalizedVec();
}

ostream& operator<<(ostream& os, const Ray &ray)
{
	os << "origin: " << ray.GetOrigin() << ", " << endl
		<< "direction: " << ray.GetDirection() << endl;
	return os;
}