#include "Light.h"
#include <algorithm>

Light::Light(LightType type)
	: type(type)
{
	SetDefaultPty();
}

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