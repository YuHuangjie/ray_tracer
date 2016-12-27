#include "Phong.h"
#include "Define.h"
#include <algorithm>

Color Phong::GetColor(
	const Vector4 &incidentDir,
	const Intersection &inter,
	const vector< shared_ptr<const Light> > &lights,
	const vector< shared_ptr<const Primitive> > &objects,
	bool primary)
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
	Ray L;
	double diffuseScale, specularScale;

	// direct lighting
	vector< shared_ptr<const Light> >::const_iterator light;
	Color ambientIntensity, diffuseIntensity, specularIntensity;
	Vector4 lightDir;
	double distL2P;
	for(light = lights.cbegin(); light != lights.cend(); ++light) {

		(*light)->Illuminate(p, lightDir, ambientIntensity
			, diffuseIntensity, specularIntensity, distL2P);

		// ambient
		if (primary) {
			ret += ambientIntensity * object->GetAmbient();
		}

		// shadow ray test
		bool visible = !Assistant::Shadow(Ray(p, -lightDir), objects, distL2P);

		// apply diffuse and specular effect if not shadow found
		if (visible) {
			// diffuse 
			diffuseScale = std::max(0.0, N * (-lightDir));
			ret += diffuseIntensity * object->GetDiffuse() * diffuseScale;

			// specular
			Vector4 H = (-lightDir + V);
			H.Normalize();
			specularScale = std::max(0.0, N * H);
			specularScale = pow(specularScale, object->GetSpecularExp());
			ret += specularIntensity * object->GetSpecular() * specularScale;
		}
	}

	return ret;
}