#ifndef MESH_H
#define MESH_H

#include <string>
#include <memory>
#include <vector>

#include "Triangle.h"
#include "obj_parser\objLoader.h"
#include "Intersection.h"
#include "KDTree.h"

using namespace std;

class Mesh : public Primitive
{
public:
	Mesh(const string &file);

	Intersection Intersect(const Ray &ray, double &dist) const;

	bool IsEmpty(void) const;

public:
	unique_ptr<objLoader> obj;
	vector< shared_ptr<Triangle> > triangles;
	KDTree root;
	int triCount;
	int vertexCount;
};

#endif