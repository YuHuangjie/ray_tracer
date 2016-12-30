#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include "Intersection.h"
#include "Ray.h"
#include "Color.h"
#include "PrimitiveType.h"
#include <cstdint>

class Intersection;

class Primitive
{
public:
	Primitive();
	Primitive(PrimitiveType type);
	virtual ~Primitive() { }

	virtual Intersection Intersect(const Ray &ray, double &dist) const = 0;

	virtual void SetAmbient(const Color&);
	virtual Color GetAmbient(void) const;

	virtual void SetDiffuse(const Color&);
	virtual Color GetDiffuse(void) const;

	virtual void SetSpecular(const Color&);
	virtual Color GetSpecular(void) const;

	virtual void SetSpecularExp(const uint16_t ns);
	virtual uint16_t GetSpecularExp(void) const;

	virtual void SetReflection(const double r);
	virtual double GetReflection(void) const;

	virtual void SetRoughness(const double r);
	virtual double GetRoughness(void) const;

	virtual void SetFresnelReflection(const double);
	virtual double GetFresnelReflection(void) const;

	virtual PrimitiveType GetType(void) const;


protected:
	Color ka;	// ambient coefficient
	Color kd;	// diffuse coefficient
	Color ks;	// specular coefficient
	uint16_t ns;	// sharpness of specular reflection
	double r;	// reflection amount
	double roughness;	// 0 : smooth, 1: rough
	double fresnel;		// fresnel reflectance at normal incidence

	PrimitiveType type;

private:
	void SetDefaultMtl(void);
};


#endif