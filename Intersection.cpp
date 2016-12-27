#include "Intersection.h"

Intersection::Intersection()
	: position(0, 0, 0), normal(0, 0, 0), object(nullptr)
{}

Intersection::Intersection(
	const Point4 &pos, 
	const Vector4 &nor, 
	const shared_ptr<const Primitive> &obj)
	: position(pos), normal(nor), object(obj)
{}

void Intersection::SetPosition(const Point4 &pos)
{
	this->position = pos;
}

Point4 Intersection::GetPosition(void) const
{
	return position;
}

void Intersection::SetNormal(const Vector4 &normal)
{
	this->normal = normal;
	this->normal.Normalize();
}

Vector4 Intersection::GetNormal(void) const
{
	return normal;
}

void Intersection::SetPrimitive(const shared_ptr<const Primitive> &object)
{
	this->object = object;
}

shared_ptr<const Primitive> Intersection::GetPrimitive(void) const
{
	return object;
}

ostream& operator<< (ostream &os, const Intersection &inter)
{
	os << "Intersection point: " << inter.GetPosition() << endl
		<< "Normal: " << inter.GetNormal() << endl
		<< "Primitive: " << inter.GetPrimitive()->GetType() << endl;
	return os;
}
