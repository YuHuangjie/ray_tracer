#include "PointLight.h"

PointLight::PointLight()
	: Light(POINT_LIGHT)
	, position(0, 0, 0)
{}

PointLight::PointLight(const Point4 &pos)
	: Light(POINT_LIGHT)
	, position(pos)
{}

void PointLight::Illuminate(
	const Point4 &P, 
	Vector4 &lightDir, 
	Color &radians,
	double &dist) const
{
	lightDir = (P - position);
	dist = lightDir.Norm();
	lightDir.Normalize();
	radians = intensity / (4 * PI * dist);
}

void PointLight::SetPosition(const Point4 &pos)
{
	position = pos;
}

Point4 PointLight::GetPosition(void) const
{
	return position;
}

ostream& operator<<(ostream &os, const PointLight &pl)
{
	os << "position: " << pl.GetPosition() << endl
		<< "intensity: " << pl.GetIntensity() << endl;
	return os;
}