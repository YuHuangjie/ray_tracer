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
}

Color Primitive::GetDiffuse(void) const
{
	return kd;
}

void Primitive::SetSpecular(const Color& specular)
{
	ks = specular;
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
}