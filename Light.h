#ifndef LIGHT_H
#define LIGHT_H

#include "Color.h"
#include "LightType.h"
#include "Point4.h"
#include "Vector4.h"
#include "Define.h"

class Light
{
public:
	Light(LightType);
	virtual ~Light() {}

	virtual void Illuminate(const Point4 &, Vector4 &, Color &, Color &, Color &, double &) const = 0;

	void SetIntensity(double);
	double GetIntensity(void) const;

	void SetAmbient(const Color &);
	Color GetAmbient(void) const;

	void SetDiffuse(const Color &);
	Color GetDiffuse(void) const;

	void SetSpecular(const Color &);
	Color GetSpecular(void) const;

	LightType GetType(void) const;

protected:
	double intensity;
	Color ambient;
	Color diffuse;
	Color specular;

	LightType type;

private:
	void SetDefaultPty();
};


#endif