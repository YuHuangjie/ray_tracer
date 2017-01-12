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
	vector<Vector4> &lightDir, 
	vector<Color> &radians,
	vector<double> &dist) const
{
	lightDir.clear();
	radians.clear();
	dist.clear();

	Vector4 dir = P - position;
	double distance = dir.Norm();
	lightDir.push_back(dir.NormalizedVec());
	radians.push_back(intensity / (4 * PI * distance));
	dist.push_back(distance);
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