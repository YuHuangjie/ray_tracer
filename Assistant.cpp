#include "Assistant.h"
#include <cfloat>


bool Assistant::Shadow(
	const Ray &ray,
	const vector< shared_ptr<const Primitive> > &objects,
	const double distToLight)
{
	Intersection inter;
	vector< shared_ptr<const Primitive> >::const_iterator object;
	double dist;

	for (object = objects.cbegin(); object != objects.cend(); ++object) {
		inter = (*object)->Intersect(ray, dist); 
		if (inter.GetPrimitive() != nullptr && dist < distToLight) {
			return true;
		}
	}
	return false;
}

Intersection Assistant::Trace(
	const Ray &ray,
	const vector< shared_ptr<const Primitive> > &objects)
{
	Intersection closest;
	Intersection inter;
	double smallestDist = DBL_MAX;
	double dist;
	vector< shared_ptr<const Primitive> >::const_iterator it;
	
	for (it = objects.cbegin();	it != objects.cend(); ++it) {
		// intersect
		inter = (*it)->Intersect(ray, dist);
		// compare
		if (inter.GetPrimitive() != nullptr && dist < smallestDist) {
			smallestDist = dist;
			closest = inter;
		}
	}
	return closest;
}