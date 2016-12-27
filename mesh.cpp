#include "mesh.h"
#include <iostream>

Mesh::Mesh(const string &file)
{
	obj = unique_ptr<objLoader>(new objLoader());
	int ret = obj->load(file.c_str());	

	if (ret == 0) {
		obj.reset();		// 0 indicates read obj fail
		cerr << file << " not valid" << endl;
		return;
	}

	/*
	 * convert obj structures
	 */
	vertexCount = obj->vertexCount;
	triCount = obj->faceCount;

	obj_face *o;
	obj_material *mtl;
	Point4 v1, v2, v3;

	triangles.reserve(triCount);
	for (int i = 0; i != triCount; ++i) {
		triangles.push_back( make_shared<Triangle>() );
	}

	cout << "loading " << file << ": " << vertexCount << " vertices, "
		<< triCount << " triangles." << endl;

	for (int i = 0; i != triCount; ++i) {
		o = obj->faceList[i];		// raw data
		if (o->material_index == -1) {
			mtl = nullptr;
		}
		else {
			mtl = obj->materialList[o->material_index];
		}

		v1 = Point4 (
			obj->vertexList[o->vertex_index[0]]->e[0],
			obj->vertexList[o->vertex_index[0]]->e[1],
			obj->vertexList[o->vertex_index[0]]->e[2]);
		v2 = Point4(
			obj->vertexList[o->vertex_index[1]]->e[0],
			obj->vertexList[o->vertex_index[1]]->e[1],
			obj->vertexList[o->vertex_index[1]]->e[2]);
		v3 = Point4(
			obj->vertexList[o->vertex_index[2]]->e[0],
			obj->vertexList[o->vertex_index[2]]->e[1],
			obj->vertexList[o->vertex_index[2]]->e[2]);

		triangles[i]->SetVertices(v1, v2, v3);
		if (mtl) {
			triangles[i]->SetAmbient(Color(mtl->amb[0], mtl->amb[1], mtl->amb[2]));
			triangles[i]->SetDiffuse(Color(mtl->diff[0], mtl->diff[1], mtl->diff[2]));
			triangles[i]->SetSpecular(Color(mtl->spec[0], mtl->spec[1], mtl->spec[2]));
			triangles[i]->SetSpecularExp((uint16_t)mtl->shiny);
			triangles[i]->SetReflection(mtl->reflect);
		}
	}

	// build KD tree
	root = KDNode::Build(triangles);

	cout << file << " loaded successfully" << endl;
}

Intersection Mesh::Intersect(const Ray &ray, double &dist) const
{
	/*
	 * brute force intersection method
	 */
	/*Intersection inters;
	Intersection nearest_inters;
	double d;
	double smallest_dist = 1e5;
	
	for (int t = 0; t != triCount; ++t) {
		inters = triangles[t]->Intersect(ray, d);
		if (!inters.GetPrimitive()) {
			continue;
		}
		if (d < smallest_dist) {
			smallest_dist = d;
			nearest_inters = inters;
		}
	}
	dist = smallest_dist;
	return nearest_inters;*/

	/*
	 * use KD-tree for intersection
	 */
	Intersection inters = root->Intersect(ray, dist);

	return inters;
}

bool Mesh::IsEmpty(void) const
{
	return (obj == nullptr);
}