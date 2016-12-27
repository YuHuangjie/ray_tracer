#ifndef HELPER_H
#define HELPER_H

#include "Intersection.h"
#include "Ray.h"
#include "Primitive.h"
#include "mesh.h"
#include <vector>

class Assistant
{
public:
	static bool Shadow(
		const Ray &,
		const vector< shared_ptr<const Primitive> > &,
		const double distToLight);

	static Intersection Trace(
		const Ray &,
		const vector< shared_ptr<const Primitive> > &);

};


#endif