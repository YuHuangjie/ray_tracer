#include "Renderer.h"
#include "Assistant.h"
#include "Phong.h"
#include "CookTorrance.h"
#include "Define.h"

Renderer::Renderer()
	: depth(5)
{}

Renderer::Renderer(const uint16_t depth)
	: depth(depth)
{}

void Renderer::SetDepth(const uint16_t depth)
{
	this->depth = depth;
}

uint16_t Renderer::GetDepth(void) const
{
	return depth;
}

Color Renderer::RenderRay(
	const Ray &ray,
	const vector< shared_ptr<const Primitive> > &objects,
	const vector< shared_ptr<const Light> > &lights,
	bool globalLighting/* = false */) const
{
	if (objects.size() == 0 || lights.size() == 0) {
		return Color(0, 0, 0);
	}
	return RenderRay(ray, objects, lights, depth, globalLighting);
}

Color Renderer::RenderRay(
	const Ray &ray,
	const vector< shared_ptr<const Primitive> > &objects,
	const vector< shared_ptr<const Light> > &lights,
	uint16_t depth,
	bool globalLighting/* = false */) const
{
	// stop condition
	if (depth == 0) {
		return Color(0, 0, 0);
	}
	
	Color ret(0, 0, 0);
	// intersect with every object and find the closest intersection
	Intersection inter = Assistant::Trace(ray, objects);
	const shared_ptr<const Primitive> interObj = inter.GetPrimitive();
	
	// ray doesn't intersect with any objects
	if (interObj == nullptr) {
		return BackGroundColor;
	}

	if (!globalLighting) {
		// ambient simulates global diffuse
		ret += CookTorrance::GetColor(-ray.GetDirection(), inter, lights, objects, true);

		// recursive tracing on reflected ray
		Ray reflect;
		Vector4 V = ray.GetDirection();
		Vector4 N = inter.GetNormal();
		reflect.SetOrigin(inter.GetPosition());
		reflect.SetDirection((V - N * 2 * (V*N)).NormalizedVec());
		Color c = RenderRay(reflect, objects, lights, depth - 1, globalLighting);
		ret += c * interObj->GetReflection(); 
	}
	else {
		// local illumination
		ret += CookTorrance::GetColor(-ray.GetDirection(), inter, lights, objects, false);

		// global illumination

	}

	return ret;
}


//Color Renderer::RenderRayEnv(
//	const Ray& ray,
//	const vector< shared_ptr<Mesh> >& meshes,
//	const unique_ptr<CubeMap> &cubeMap)
//{
//	if (meshes.size() == 0 || cubeMap->IsEmpty())
//		return Color(0, 0, 0);
//
//	Color ret(0, 0, 0);
//	
//	// intersect with all meshes and find the closest intersection
//	Intersection inter = HelperFunction::ClosestIntersect(
//		ray,
//		vector<Primitive*>(),
//		meshes);
//	const Primitive *triangle = inter.GetPrimitive();
//
//	// intersect with no triangles
//	if (triangle == nullptr) {
//		// draw cube map
//		ret = cubeMap->GetSpecularLight(ray);
//
//		return ret;
//	}
//
//	// give perturbation to intersection point
//	inter.SetPosition(inter.GetPosition() + inter.GetNormal() * EPS);
//
//	// Reflected ray
//	Ray reflect;
//	Vector4 V = ray.GetDirection();
//	Vector4 N = inter.GetNormal();
//	reflect.SetDirection((V - N * 2 * (V*N)).NormalizedVec());
//	reflect.SetOrigin(inter.GetPosition());
//
//	// block test
//	bool block = HelperFunction::Block(reflect, meshes);
//
//	if (block) {
//		return ret;
//	}
//
//	// specular illumination
//	if (!block) {
//		ret = cubeMap->GetSpecularLight(reflect);
//	}
//
//	// diffuse illumination
//	ret += cubeMap->GetDiffuseLight(-ray.GetDirection(), inter, meshes);
//
//	return ret;
//}
