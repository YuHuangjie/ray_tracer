#include "KDTree.h"
#include "Define.h"

KDNode::KDNode()
	: left(nullptr), right(nullptr)
{}

unique_ptr<KDNode> KDNode::Build(
	const vector< shared_ptr<Triangle> >& triangles, 
	int thresh/* =5 */,
	int depth)
{
	unique_ptr<KDNode> root = make_unique<KDNode>();
	root->depth = depth;
	root->triangles = triangles;
	root->bbox = BBox();
	size_t triCount = triangles.size();

	// no triangles at all
	if (triCount == 0) {
		return root;
	}
	
	// get a bounding box of all triangles
	root->bbox = BBox::FromTriangles(triangles);

	// end when triangles number is below threshold
	if (triCount <= thresh) {
		return root;
	}

	// find midpoints of all triangles
	Point4 midAll(0, 0, 0);
	Point4 midOne(0, 0, 0);
	vector< shared_ptr<Triangle> >::const_iterator it;

	for (it = triangles.cbegin(); it != triangles.cend(); ++it) {
		midOne = (*it)->GetMidPoint();
		midAll.x += midOne.x;
		midAll.y += midOne.y;
		midAll.z += midOne.z;
	}
	midAll.x /= triCount;
	midAll.y /= triCount;
	midAll.z /= triCount;

	// split triangles based on their midpoints side of avg on longest axis
	vector< shared_ptr<Triangle> > left_node;
	vector< shared_ptr<Triangle> > right_node;
	Axis longestAxis = root->bbox.GetLongestAxis();

	for (it = triangles.cbegin(); it != triangles.cend(); ++it) {
		switch (longestAxis)
		{
		case X_Axis:
			midAll.x > (*it)->GetMidPoint().x ? 
				left_node.push_back(*it) : right_node.push_back(*it);
			break;

		case Y_Axis:
			midAll.y > (*it)->GetMidPoint().y ?
				left_node.push_back(*it) : right_node.push_back(*it); 
			break;

		case Z_Axis:
			midAll.z > (*it)->GetMidPoint().z ?
				left_node.push_back(*it) : right_node.push_back(*it); 
			break;

		default:
			break;
		}
	}

	Assertion(left_node.size() > 0 && right_node.size() > 0, "unbalanced left and right node");

	// recurse down to left and right nodes
	root->left = KDNode::Build(left_node, depth + 1, thresh);
	root->right = KDNode::Build(right_node, depth + 1, thresh);

	return root;
}

Intersection KDNode::Intersect(const Ray &ray, double &dist) const
{
	Intersection intersClosest;
	Intersection intersLeft;
	Intersection intersRight;
	double dLeft, dRight;

	// check if ray intersect with the bounding box
	if (!bbox.Intersect(ray, dLeft)) {
		return intersClosest;
	}

	if (left || right) {
		Assertion(left && right, "");
		// recurse down to left and right child
		intersLeft = left->Intersect(ray, dLeft);
		intersRight = right->Intersect(ray, dRight);
		// compare left and right
		if (!intersLeft.GetPrimitive() && !intersRight.GetPrimitive()) {
			// doesn't intersect with left and right
			intersClosest.SetPrimitive(nullptr);
		}
		else if (!intersLeft.GetPrimitive()) {
			// intersect with right only
			intersClosest = intersRight;
			dist = dRight;
		}
		else if (!intersRight.GetPrimitive()) {
			// intersect with left only
			intersClosest = intersLeft;
			dist = dLeft;
		}
		else {
			// intersect with both
			if (dLeft < dRight) {
				intersClosest = intersLeft;
				dist = dLeft;
			}
			else {
				intersClosest = intersRight;
				dist = dRight;
			}
		}
	}
	else {
		// we have reached a leaf
		Intersection inters;
		double d;
		double smallestDist = 1e6;

		for (int t = 0; t != triangles.size(); ++t) {
			inters = triangles[t]->Intersect(ray, d);
			if (!inters.GetPrimitive()) {
				continue;
			}
			if (d < smallestDist) {
				smallestDist = d;
				intersClosest = inters;
			}
		}

		dist = smallestDist;
	}

	return intersClosest;
}


ostream& operator<<(ostream& os, const unique_ptr<KDNode> &node)
{
	for (int i = 0; i != node->depth; ++i) {
		os << "\t";
	}

	if (node != nullptr) {
		os << node->bbox << endl;
	}
	if (node->left != nullptr) {
		os << node->left << endl;
	}
	if (node->right != nullptr) {
		os << node->right << endl;
	}

	return os;
}