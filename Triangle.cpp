#include "Triangle.h"
#include "Define.h"

Triangle::Triangle()
	: Primitive(TRIANGLE)
{	}

Triangle::Triangle(const Point4 &v1, const Point4 &v2, const Point4 &v3)
	: Primitive(TRIANGLE), v1(v1), v2(v2), v3(v3)
{
	Vector4 u = v1 - v2;
	Vector4 v = v1 - v3;
	normal = u.Cross(v).NormalizedVec();
}

void Triangle::SetVertices(const Point4 &p1, const Point4 &p2, const Point4 &p3)
{
	v1 = p1;
	v2 = p2;
	v3 = p3;

	// calculate normal
	Vector4 u = v1 - v2;
	Vector4 v = v1 - v3;
	normal = u.Cross(v).NormalizedVec();
}

void Triangle::GetVertices(Point4 &p1, Point4 &p2, Point4 &p3)
{
	p1 = v1;
	p2 = v2;
	p3 = v3;
}

void Triangle::SetNormal(const Vector4 &normal)
{
	this->normal = normal;
	this->normal.Normalize();

	// validate
	Vector4 u = v1 - v2;
	Vector4 v = v1 - v3;
	Assertion(abs(normal*u) < EPS && abs(normal*v) < EPS, "Triangle normal not perpendicular to edges");
}

void Triangle::GetNormal(Vector4 &n)
{
	n = normal;
}

Intersection Triangle::Intersect(const Ray &ray, double &dist) const
{
	Vector4 u, v;	// triangle vectors
	Intersection inters;

	// get triangle edge vectors and plane normal
	u = v2 - v1;
	v = v3 - v1;
	if (normal.Norm() < EPS) {	// triangle is degenerate
		return inters;
	}

	Vector4 w0 = ray.GetOrigin() - v1;
	double a = -normal * w0;
	double b = normal * ray.GetDirection();
	if (abs(b) < EPS) {			// ray is parallel to triangle plane
		return inters;
	}

	// get intersect point of ray with triangle plane
	double r = a / b;
	if (r <= 0.0) {			// ray goes away from triangle plane
		return inters;
	}

	//if (r < 0.00001) {
	//	int c = 0;
	//}

	// intersect point of ray with triangle plane
	Point4 p = ray.GetOrigin() + ray.GetDirection() * r;
	dist = r;

	// is intersect point inside triangle?
	double uu, uv, vv, wu, wv, D;
	Vector4 w;
	uu = u * u;
	uv = u * v;
	vv = v * v;
	w = p - v1;
	wu = w * u;
	wv = w * v;
	D = uv * uv - uu * vv;

	// get and test parametric coords
	double s, t;
	s = (uv * wv - vv * wu) / D;
	if (s <= 0.0 || s >= 1.0) {	// outside triangle
		return inters;
	}
	t = (uv * wu - uu * wv) / D;
	if (t <= 0.0 || (s + t) >= 1.0) {	// outside triangle
		return inters;
	}

	inters.SetNormal(normal);
	inters.SetPosition(p + normal * EPS);
	inters.SetPrimitive(shared_from_this());
	return inters; 
}

Point4 Triangle::GetMidPoint(void) const
{
	return Point4(
		(v1.x + v2.x + v3.x) / 3,
		(v1.y + v2.y + v3.y) / 3,
		(v1.z + v2.z + v3.z) / 3);
}


ostream& operator<<(ostream &os, const Triangle &t)
{
	os << "vertex1: " << t.v1 
		<< ", vertex2: " << t.v2 
		<< ", vertex3: " << t.v3 << endl;
	return os;
}