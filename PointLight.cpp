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
	Color &ambientIntensity,
	Color &diffuseIntensity,
	Color &specularIntensity,
	double &dist) const
{
	lightDir = (P - position);
	dist = lightDir.Norm();
	lightDir.Normalize();
	ambientIntensity = ambient * intensity / (4 * PI * dist);
	diffuseIntensity = diffuse * intensity / (4 * PI * dist);
	specularIntensity = specular * intensity / (4 * PI * dist);
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
		<< "ambient: " << pl.GetAmbient() << endl
		<< "diffuse: " << pl.GetDiffuse() << endl
		<< "specular: " << pl.GetSpecular() << endl;
	return os;
}