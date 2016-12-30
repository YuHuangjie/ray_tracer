#ifndef PHONG_H
#define PHONG_H

#include <vector>
#include "Color.h"
#include "Ray.h"
#include "Intersection.h"
#include "PointLight.h"
#include "Assistant.h"

using namespace std;

class Phong
{
public:

	static Color GetColor(
		const Vector4 &incidentDir, // point away from surface
		const Intersection &inter, 
		const vector< shared_ptr<const Light> > &lights,
		const vector< shared_ptr<const Primitive> > &objects,
		bool useAmbient);

};


#endif