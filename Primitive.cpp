#include "Primitive.h"

Primitive::Primitive() 
	: type(NOT_OBJECT)
{
	SetDefaultMtl();
}

Primitive::Primitive(PrimitiveType type)
	: type(type)
{
	SetDefaultMtl();
}


void Primitive::SetAmbient(const Color& ambient)
{
	ka = ambient;
}

Color Primitive::GetAmbient(void) const
{
	return ka;
}

void Primitive::SetDiffuse(const Color& diffuse)
{
	kd = diffuse;
	ks = ks.Clamp(Color(1, 1, 1) - kd);
}

Color Primitive::GetDiffuse(void) const
{
	return kd;
}

void Primitive::SetSpecular(const Color& specular)
{
	ks = specular;
	kd = kd.Clamp(Color(1, 1, 1) - ks);
}

Color Primitive::GetSpecular(void) const
{
	return ks;
}

void Primitive::SetSpecularExp(const uint16_t ns)
{
	this->ns = ns;
}

uint16_t Primitive::GetSpecularExp(void) const
{
	return ns;
}

void Primitive::SetReflection(const double r)
{
	this->r = r;
}

double Primitive::GetReflection(void) const
{
	return r;
}

void Primitive::SetRoughness(const double rough)
{
	if (rough > 1) {
		roughness = 1;
	}
	else if (rough < 1e-5) {
		roughness = 1e-5;
	}
	else {
		roughness = rough;
	}
}

double Primitive::GetRoughness(void) const
{
	return roughness;
}

void Primitive::SetFresnelReflection(const double f)
{
	fresnel = f;
}

double Primitive::GetFresnelReflection(void) const
{
	return fresnel;
}

PrimitiveType Primitive::GetType(void) const
{
	return type;
}

void Primitive::SetDefaultMtl(void)
{
	ka = Color(0, 0, 0);
	kd = Color(0, 0, 0);
	ks = Color(0, 0, 0);
	ns = 1;
	r = 0;
	roughness = 0.3;
	fresnel = 0.8;
}