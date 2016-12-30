#include "Phong.h"
#include "Define.h"
#include <algorithm>

Color Phong::GetColor(
	const Vector4 &incidentDir,
	const Intersection &inter,
	const vector< shared_ptr<const Light> > &lights,
	const vector< shared_ptr<const Primitive> > &objects,
	bool useAmbient)
{
	// check intersection is valid
	if (inter.GetPrimitive() == nullptr) {
		return Color(0, 0, 0);
	}

	Color ret(0, 0, 0);
	const Point4 p = inter.GetPosition();
	const Vector4 N = inter.GetNormal();
	const shared_ptr<const Primitive> object = inter.GetPrimitive();
	const Vector4 V = incidentDir;
	double diffuseScale, specularScale;

	// direct lighting
	vector< shared_ptr<const Light> >::const_iterator light;
	Color lightRadiance;
	Vector4 lightDir;
	double distL2P;
	for(light = lights.cbegin(); light != lights.cend(); ++light) {

		(*light)->Illuminate(p, lightDir, lightRadiance, distL2P);

		// ambient
		if (useAmbient) {
			ret += lightRadiance * object->GetAmbient();
		}

		// shadow ray test
		bool visible = !Assistant::Shadow(Ray(p, -lightDir), objects, distL2P);

		// apply diffuse and specular effect if not shadow found
		if (visible) {
			// diffuse 
			diffuseScale = std::max(0.0, N * (-lightDir));
			ret += lightRadiance * object->GetDiffuse() * diffuseScale;

			// specular
			Vector4 H = (-lightDir + V);
			H.Normalize();
			specularScale = std::max(0.0, N * H);
			specularScale = pow(specularScale, object->GetSpecularExp());
			ret += lightRadiance * object->GetSpecular() * specularScale;
		}
	}

	return ret;
}