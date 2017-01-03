#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include <random>

#include "Color.h"
#include "Ray.h"
#include "Primitive.h"
#include "PointLight.h"
//#include "mesh.h"
//#include "cube_map.h"

using namespace std;

class Renderer
{
public:
	Renderer();
	Renderer(const uint16_t depth);

	Color RenderRay(
		const Ray&,
		const vector< shared_ptr<const Primitive> > &,
		const vector< shared_ptr<const Light> > &,
		bool globalLighting = false);


	//Color RenderRayEnv(
	//	const Ray&,
	//	const vector< shared_ptr<Mesh> > &,
	//	const unique_ptr<CubeMap> &
	//	);

	void SetDepth(const uint16_t depth);
	uint16_t GetDepth(void) const;

	void SetIndirectDiffuseSample(const uint16_t &sample);
	uint16_t GetIndirectDiffuseSample(void) const;

	void SetIndirectSpecularSample(const uint16_t &sample);
	uint16_t GetIndirectSpecularSample(void) const;

private:
	Color RenderRay(
		const Ray &,
		const vector< shared_ptr<const Primitive> > &,
		const vector< shared_ptr<const Light> > &,
		uint16_t depth,
		bool globalLight = false);

	void CreateCoordinateSystem(const Vector4 &N, Vector4 &Nt, Vector4 &Nb) const;
	Vector4 UniformSampleHemisphere(const double &r1, const double &r2) const;


	uint16_t depth;
	uint16_t indDiffuseSample;
	uint16_t indSpecularSample;
	
	// random generator
	default_random_engine generator;
	uniform_real_distribution<double> distribution;
};


#endif