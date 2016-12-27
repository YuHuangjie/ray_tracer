#include "InfinitePlane.h"
#include "Define.h"
#include <cmath>

using namespace std;

InfinitePlane::InfinitePlane()
	: Primitive(PrimitiveType::INF_PLANE)
	, fixPoint(0, 0, 0)
	, normal(0, 0, 1)
{}

InfinitePlane::InfinitePlane(const Point4 &p, const Vector4 &normal)
	: Primitive(PrimitiveType::INF_PLANE)
	, fixPoint(p)
{
	this->normal = normal;
	this->normal.Normalize();
}

Intersection InfinitePlane::Intersect(const Ray &incident, double &dist) const
{
	Point4 ori = incident.GetOrigin();
	Vector4 dir = incident.GetDirection();

	// incident is perpendicular with plane
	if (abs(normal * dir) < EPS) {
		return Intersection();
	}

	// distance from origin to intersection point
	double scale = (fixPoint - ori)*normal / (dir * normal);
	Intersection inter;

	if (scale > 0) {
		// add perturbation
		Point4 p = ori + dir * scale + normal * EPS;	
		inter.SetPosition(p);
		inter.SetNormal(normal);
		inter.SetPrimitive(shared_from_this());
		dist = scale;
	}
	return inter;
}

Point4 InfinitePlane::GetFixPoint(void) const
{
	return fixPoint;
}

Vector4 InfinitePlane::GetNormal(void) const
{
	return normal;
}

ostream& operator<<(ostream& os, const InfinitePlane &plane)
{
	os << "fix point: " << plane.GetFixPoint() << endl
		<< "normal: " << plane.GetNormal() << endl;
	return os;
}