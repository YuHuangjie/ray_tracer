#ifndef KDTREE_H
#define KDTREE_H

#include "BBox.h"
#include "Triangle.h"
#include <vector>
#include <memory>
#include <iostream>

using namespace std;

class KDNode
{
public:
	KDNode();

	static unique_ptr<KDNode> Build(
		const vector< shared_ptr<Triangle> > &, 
		int thresh = 5,
		int depth = 0
		);

	Intersection Intersect(const Ray &ray, double &dist) const;

	friend ostream& operator<<(ostream&, const unique_ptr<KDNode> &);

private:
	
	BBox bbox;
	unique_ptr<KDNode> left;
	unique_ptr<KDNode> right;
	vector< shared_ptr<Triangle> > triangles;

	int depth;	// used mainly for structured output
};

typedef unique_ptr<KDNode> KDTree;

#endif