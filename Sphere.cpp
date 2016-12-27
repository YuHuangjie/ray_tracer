#include "Sphere.h"
#include "Define.h"
#include <cmath>
#include <algorithm>

Sphere::Sphere()
	: Primitive(SPHERE)
	, center(0, 0, 0), radius(1.0)
{}

Sphere::Sphere(const Point4 &p, double r)
	: Primitive(SPHERE)
{
	if (r < 0) {
		r = 1;
	}

	center = p;
	radius = r;
}

Point4 Sphere::GetCenter(void) const
{
	return center;
}

double Sphere::GetRadius(void) const
{
	return radius;
}

Intersection Sphere::Intersect(const Ray &ray, double &dist) const
{
	Vector4 deltaP = ray.GetOrigin() - center;
	Vector4 dir = ray.GetDirection();
	double delta = pow((dir*deltaP), 2) - deltaP*deltaP + radius*radius;

	// if ray starts inside sphere
	if (deltaP * deltaP < radius * radius) {
		return Intersection();
	}

	if (delta < 0) {
		// find no intersection
		return Intersection();
	}
	else {
		double scale1 = -(dir*deltaP) + sqrt(delta);
		double scale2 = -(dir*deltaP) - sqrt(delta);
		
		double scale;
		// find the smallest positive scale
		if (scale1 < 0 && scale2 > 0)
			scale = scale2;
		else if (scale1 > 0 && scale2 < 0)
			scale = scale1;
		else if (scale1 > 0 && scale2 > 0)
			scale = std::min(scale1, scale2);
		else
			scale = -1;

		if (scale < 0) {
			return Intersection();
		}
		else {
			Intersection ret;
			Point4 P = ray.GetOrigin() + dir * scale;
			ret.SetNormal((P - center).NormalizedVec());
			ret.SetPosition(P + ret.GetNormal() * EPS);
			ret.SetPrimitive(shared_from_this());
			dist = scale;
			return ret;
		}
	}
}

ostream& operator<<(ostream &os, const Sphere &s)
{
	os << "center: " << s.GetCenter() << endl
		<< "radius: " << s.GetRadius() << endl;
	return os;
}