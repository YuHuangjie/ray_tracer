#include "Renderer.h"
#include "Assistant.h"
#include "Phong.h"
#include "CookTorrance.h"
#include "Define.h"

Renderer::Renderer()
	: depth(5)
	, indDiffuseSample(16)
	, indSpecularSample(16)
	, distribution(0, 1)
{
}

Renderer::Renderer(const uint16_t depth)
	: depth(depth)
	, indDiffuseSample(16)
	, indSpecularSample(16)
	, distribution(0, 1)
{}

void Renderer::SetDepth(const uint16_t depth)
{
	this->depth = depth;
}

uint16_t Renderer::GetDepth(void) const
{
	return depth;
}

void Renderer::SetIndirectDiffuseSample(const uint16_t &sample)
{
	indDiffuseSample = sample;
}

uint16_t Renderer::GetIndirectDiffuseSample(void) const
{
	return indDiffuseSample;
}

void Renderer::SetIndirectSpecularSample(const uint16_t &sample)
{
	indSpecularSample = sample;
}

uint16_t Renderer::GetIndirectSpecularSample(void) const
{
	return indSpecularSample;
}

Color Renderer::RenderRay(
	const Ray &ray,
	const vector< shared_ptr<const Primitive> > &objects,
	const vector< shared_ptr<const Light> > &lights,
	bool globalLighting/* = false */)
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
	bool globalLighting/* = false */)
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
		ret += CookTorrance::GetColor(-ray.GetDirection(), inter, lights, objects, true) / PI;

		// recursive tracing on reflected ray
		Ray reflect;
		Vector4 V = -ray.GetDirection();
		Vector4 N = inter.GetNormal();
		reflect.SetOrigin(inter.GetPosition());
		reflect.SetDirection((-V + N * 2 * (V*N)).NormalizedVec());
		Color c = RenderRay(reflect, objects, lights, depth - 1, globalLighting);
		ret += c * interObj->GetReflection() / PI; 
	}
	else {
		// local illumination
		Color direct = CookTorrance::GetColor(-ray.GetDirection(), inter, lights, objects, false);

		// global diffuse illumination
		Color globalDiffuse(0, 0, 0);
		{
			double PDF = 1.0 / (2 * PI);
			Vector4 Nt, Nb;
			Vector4 N = inter.GetNormal();
			CreateCoordinateSystem(N, Nt, Nb);

			for (uint16_t n = 0; n != indDiffuseSample; ++n) {
				double r1 = distribution(generator);
				double r2 = distribution(generator);
				Vector4 sample = UniformSampleHemisphere(r1, r2);
				Vector4 sampleWorld = Nb*sample.x + N*sample.y + Nt*sample.z;
				Ray sampleRay(inter.GetPosition(), sampleWorld);
				globalDiffuse += RenderRay(sampleRay, objects, lights, depth - 1, globalLighting) / PDF * r1;
			}
			globalDiffuse = globalDiffuse / indDiffuseSample * interObj->GetDiffuse();
		}

		// global specular
		Color globalSpecular(0, 0, 0);
		{
			double roughness = interObj->GetRoughness();
			double PDF = 1;		// don't know how to calculate
			Vector4 Rt, Rb, R;
			Vector4 N = inter.GetNormal();
			Vector4 V = -ray.GetDirection();
			R = (-V + N * 2*(V*N)).NormalizedVec();
			CreateCoordinateSystem(R, Rt, Rb);

			for (uint16_t n = 0; n != indSpecularSample; ++n) {
				double r1 = 1 + (distribution(generator) - 1) * roughness;
				double r2 = distribution(generator);
				Vector4 sample = UniformSampleHemisphere(r1, r2);
				Vector4 sampleWorld = Rb*sample.x + R*sample.y + Rt*sample.z;
				if (sampleWorld * N <= 0) {
					// reflected ray under surface
					continue;
				}

				Ray sampleRay(inter.GetPosition(), sampleWorld);
				globalSpecular += RenderRay(sampleRay, objects, lights, depth - 1, globalLighting) * PI;
			}
			globalSpecular = globalSpecular / indSpecularSample * interObj->GetSpecular();

		}

		// add up local & global illumination
		ret = direct / PI + globalDiffuse + globalSpecular;
	}

	return ret;
}

void Renderer::CreateCoordinateSystem(const Vector4 &N, Vector4 &Nt, Vector4 &Nb) const
{
	if (std::abs(N.x) > abs(N.y))
		Nt = Vector4(N.z, 0, -N.x).NormalizedVec();
	else
		Nt = Vector4(0, -N.z, N.y).NormalizedVec();
	Nb = N.Cross(Nt);
}

Vector4 Renderer::UniformSampleHemisphere(const double &r1, const double &r2) const
{
	double sinTheta = sqrt(1 - r1 * r1);
	double phi = 2 * PI * r2;
	double x = sinTheta * cos(phi);
	double z = sinTheta * sin(phi);
	return Vector4(x, r1, z);
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
