#ifndef COOKTORRANCE_H
#define COOKTORRANCE_H


#include <vector>
#include "Color.h"
#include "Ray.h"
#include "Intersection.h"
#include "PointLight.h"
#include "Assistant.h"

using namespace std;

class CookTorrance
{
public:
	static Color GetColor(
		const Vector4 &incidentDir,
		const Intersection &inter,
		const vector< shared_ptr<const Light> > &lights,
		const vector< shared_ptr<const Primitive> > &objects,
		bool ambient);
};


#endif