#include "Light.h"
#include <algorithm>

Light::Light(LightType type)
	: type(type)
{
	SetDefaultPty();
}

//void Light::SetAmbient(const Color &color)
//{
//	ambient = color;
//	ambient.Clamp();
//}
//
//Color Light::GetAmbient(void) const
//{
//	return ambient;
//}
//
//void Light::SetDiffuse(const Color &color)
//{
//	diffuse = color;
//	diffuse.Clamp();
//}
//
//Color Light::GetDiffuse(void) const
//{
//	return diffuse;
//}
//
//void Light::SetSpecular(const Color &color)
//{
//	specular = color;
//	specular.Clamp();
//}
//
//Color Light::GetSpecular(void) const
//{
//	return specular;
//}

LightType Light::GetType(void) const
{
	return type;
}

void Light::SetIntensity(const Color &i)
{
	intensity = i;
	intensity.Clamp();
}

Color Light::GetIntensity(void) const
{
	return intensity;
}

void Light::SetDefaultPty(void)
{
	intensity = Color(0, 0, 0);
}