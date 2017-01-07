#ifndef LIGHT_H
#define LIGHT_H

#include "Color.h"
#include "LightType.h"
#include "Point4.h"
#include "Vector4.h"
#include "Define.h"
#include <vector>

class Light
{
public:
	Light(LightType);
	virtual ~Light() {}

	virtual void Illuminate(const Point4 &, vector<Vector4> &, vector<Color> &, vector<double> &) const = 0;

	void SetIntensity(const Color&);
	Color GetIntensity(void) const;

	LightType GetType(void) const;

protected:
	Color intensity;
	LightType type;

private:
	void SetDefaultPty();
};


#endif