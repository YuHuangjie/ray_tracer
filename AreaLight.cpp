#include "AreaLight.h"

AreaLight::AreaLight()
	: Light(AREA_LIGHT)
{
	area_center = Point4(0, 0, 0);
	area_normal = Vector4(0, 0, 1);
	area_rightdir = Vector4(0, 1, 0);
	area_width = 1;
	area_height = 1;
	wSample = 0;
	hSample = 0;

	samplePositions = nullptr;
}

AreaLight::AreaLight(const Point4 &cen, const Vector4 &norm, const double &w, const double &h)
	: Light(AREA_LIGHT)
{
	area_center = cen;
	area_normal = norm;
	area_normal.Normalize();
	
	// construct default right direction (always on y-z plane)
	area_rightdir.x = 0;
	area_rightdir.y = -norm.z;
	area_rightdir.z = norm.y;
	area_rightdir.Normalize();

	area_width = (w <= 0 ? 1 : w);
	area_height = (h <= 0 ? 1 : h);

	samplePositions = nullptr;
}


void AreaLight::Illuminate(
	const Point4 &P,
	vector<Vector4> &lightDir,
	vector<Color> &radiance,
	vector<double> &dist) const
{
	if (samplePositions == nullptr) {
		lightDir.clear();
		radiance.clear();
		dist.clear(); 
		return;
	}

	Vector4 dir;
	Color color;
	lightDir.clear();
	radiance.clear();
	dist.clear();

	for (uint16_t i = 0; i != wSample * hSample; i++)
	{
		Point4 lightPos = samplePositions[i];
		dir = P - lightPos;
		double distance = dir.Norm();
		dir.Normalize();
		// Lights only emit on semisphere
		if (dir*area_normal < EPS)
		{
			color = Color(0, 0, 0);
		}
		else
		{
			color = intensity / (4 * PI * distance);
		}
		lightDir.push_back(dir);
		radiance.push_back(color);
		dist.push_back(distance);
	}
}


//void AreaLight::SetCenter(const Point4 &cen)
//{
//	area_center = cen;
//}
//
//void AreaLight::SetNormal(const Vector4 &nor)
//{
//	area_normal = nor;
//}
//
//void AreaLight::SetrightDir(const Vector4 &lef)
//{
//	area_rightdir = lef;
//}
//
//void AreaLight::SetWH(const double &w, const double &h)
//{
//	area_width = w;
//	area_height = h;
//}
//
void AreaLight::SetSample(const uint16_t nx, const uint16_t ny)
{
	wSample = (nx == 0 ? 5 : nx);
	hSample = (ny == 0 ? 5 : ny);
	samplePositions = make_unique<Point4[]>(wSample * hSample);

	// uniform sample process
	double dw = area_width / (wSample + 1);
	double dh = area_height / (hSample + 1);
	Vector4 area_updir = area_normal.Cross(area_rightdir);
	Point4 leftTop = area_center + area_updir * (area_height / 2) - area_rightdir * (area_width / 2);

	for (uint16_t w = 0; w != wSample; ++w) {
		for (uint16_t h = 0; h != hSample; ++h) {
			uint16_t index = w + h * wSample;
			Point4 pos = leftTop + area_rightdir * (w*dw) - area_updir * (h*dh);
			samplePositions[index] = pos;
		}
	}

}


ostream& operator<<(ostream &os, const AreaLight &light)
{
	os << "rectangle area light\n"
		<< "center: " << light.area_center << endl
		<< "normal: " << light.area_normal << endl
		<< "right dir: " << light.area_rightdir << endl
		<< "sample: " << light.wSample << "x" << light.hSample;
	return os;
}