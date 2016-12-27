#include "BBox.h"
#include "Define.h"
#include <algorithm>

BBox::BBox()
	: x_min(0), x_max(0),
	y_min(0), y_max(0),
	z_min(0), z_max(0)
{}

BBox::BBox(const BBox &box)
{
	*this = box;
}

void BBox::SetX(double min, double max)
{
	Assertion(min <= max, "error setting bounding box X axis");

	if (min == max) {
		max += EPS;
	}
	x_min = min;
	x_max = max;
}

void BBox::SetY(double min, double max)
{
	Assertion(min <= max, "error setting bounding box Y axis");

	if (min == max) {
		max += EPS;
	}
	y_min = min;
	y_max = max;
}

void BBox::SetZ(double min, double max)
{
	Assertion(min <= max, "error setting bounding box Z axis");

	if (min == max) {
		max += EPS;
	}
	z_min = min;
	z_max = max;
}

void BBox::GetX(double &min, double &max) const
{
	min = x_min;
	max = x_max;
}

void BBox::GetY(double &min, double &max) const
{
	min = y_min;
	max = y_max;
}

void BBox::GetZ(double &min, double &max) const
{
	min = z_min;
	max = z_max;
}


Axis BBox::GetLongestAxis(void) const
{
	double x_diff = x_max - x_min;
	double y_diff = y_max - y_min;
	double z_diff = z_max - z_min;

	if (x_diff > y_diff && x_diff > z_diff) {
		return X_Axis;
	}
	else if (y_diff > x_diff && y_diff > z_diff) {
		return Y_Axis;
	}
	else {
		return Z_Axis;
	}
}

BBox BBox::FromTriangles( const vector< shared_ptr<Triangle> > &triangles )
{
	BBox bbox;
	
	Assertion(triangles.size() > 0, "No triangles found");

	vector< shared_ptr<Triangle> >::const_iterator it;
	bbox = BBox::FromTriangle(triangles[0]);

	// merge bounding box one by one
	for (it = triangles.cbegin() + 1; it != triangles.cend(); ++it) {
		bbox = BBox::Merge( bbox, BBox::FromTriangle(*it) );
	}

	return bbox;
}

BBox BBox::FromTriangle(shared_ptr<Triangle> triangle)
{
	BBox bbox;
	double x_min, x_max;
	double y_min, y_max;
	double z_min, z_max;
	Point4 p1, p2, p3;

	// get vertices of triangle
	triangle->GetVertices(p1, p2, p3);

	// min
	x_min = min(p1.x, min(p2.x, p3.x));
	y_min = min(p1.y, min(p2.y, p3.y));
	z_min = min(p1.z, min(p2.z, p3.z));

	// max
	x_max = max(p1.x, max(p2.x, p3.x));
	y_max = max(p1.y, max(p2.y, p3.y));
	z_max = max(p1.z, max(p2.z, p3.z));

	bbox.SetX(x_min, x_max);
	bbox.SetY(y_min, y_max);
	bbox.SetZ(z_min, z_max);

	return bbox;
}

BBox BBox::Merge(const BBox &box1, const BBox &box2)
{
	BBox bbox;

	bbox.SetX(
		min(box1.x_min, box2.x_min), 
		max(box1.x_max, box2.x_max));
	bbox.SetY(
		min(box1.y_min, box2.y_min), 
		max(box1.y_max, box2.y_max));
	bbox.SetZ(
		min(box1.z_min, box2.z_min),
		max(box1.z_max, box2.z_max));

	return bbox;
}

bool BBox::Intersect(const Ray &ray, double &dist) const
{
	Point4 origin = ray.GetOrigin();
	Vector4 dir = ray.GetDirection();

	double tx_1 = (x_min - origin.x) / dir.x;
	double tx_2 = (x_max - origin.x) / dir.x;
	double ty_1 = (y_min - origin.y) / dir.y;
	double ty_2 = (y_max - origin.y) / dir.y;
	double tz_1 = (z_min - origin.z) / dir.z;
	double tz_2 = (z_max - origin.z) / dir.z;

	double tmin = max(max(min(tx_1, tx_2), min(ty_1, ty_2)), min(tz_1, tz_2));
	double tmax = min(min(max(tx_1, tx_2), max(ty_1, ty_2)), max(tz_1, tz_2));
	
	// the whole bounding box is behind the ray
	if (tmax < 0) {
		return false;
	}

	// ray doesn't intersect bbox
	if (tmin > tmax) {
		return false;
	}

	if (tmin > 0) {
		dist = tmin;
	}
	else {
		dist = tmax;
	}

	return true;
}


bool BBox::IsValid(void) const
{
	return ((x_min < x_max) && (y_min < y_max) && (z_min < z_max));
}


ostream& operator<<(ostream& os, const BBox &bbox)
{
	os << "[x: " << bbox.x_min << ", " << bbox.x_max << "]"
		<< "[y: " << bbox.y_min << ", " << bbox.y_max << "]"
		<< "[z: " << bbox.z_min << ", " << bbox.z_max << "]";
	return os;
}