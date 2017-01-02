#include "CookTorrance.h"
#include "Define.h"
#include <algorithm>

Color CookTorrance::GetColor(
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
	const Vector4 N = inter.GetNormal();
	const Vector4 V = incidentDir;
	Vector4 L;
	const Point4 P = inter.GetPosition();
	const shared_ptr<const Primitive> object = inter.GetPrimitive();
	double roughnessValue = object->GetRoughness();
	double F0 = object->GetFresnelReflection();
	Color kd = object->GetDiffuse();
	Color ks = object->GetSpecular();

	// direct lighting
	vector< shared_ptr<const Light> >::const_iterator light;
	Color lightRadiance;
	double distL2P;

	for (light = lights.cbegin(); light != lights.cend(); ++light) {

		(*light)->Illuminate(P, L, lightRadiance, distL2P);

		// ambient
		if (useAmbient) {
			ret += lightRadiance * object->GetAmbient();
		}

		// showdow ray test
		bool visible = !Assistant::Shadow(Ray(P, -L), objects, distL2P);

		// apply cook torrance model
		if (visible) {
			// calculate intermediate values
			Vector4 H = (V - L);
			H.Normalize();
			double NdotH = max(N * H, 0.0);
			double NdotV = max(N * V, 0.0);
			double NdotL = max(N * (-L), 0.0);
			double VdotH = max(V * H, 0.0);
			double LdotH = max(H * (-L), 0.0);
			double mSquare = roughnessValue * roughnessValue;

			// geometric attenuation
			double NH2 = 2.0 * NdotH;
			double g1 = (NdotV * NH2) / VdotH;
			double g2 = (NdotL * NH2) / VdotH;
			double geoAtt = min(1.0, min(g1, g2));

			// roughness (or: microfacet distribution function)
			// beckman distribution function
			double r1 = 1.0 / (PI * mSquare * pow(NdotH, 4.0));
			double r2 = (NdotH * NdotH - 1.0) / (mSquare * NdotH * NdotH);
			double roughness = r1 * exp(r2);

			// fresnel
			// schlick approximation
			double fresnel = pow(1.0 - VdotH, 5.0);
			fresnel *= (1.0 - F0);
			fresnel += F0;

			double specular = (fresnel * geoAtt * roughness) / (NdotV * NdotL);
			
			ret += lightRadiance * NdotL * (kd + ks * specular);
		}
	}

	return ret;
}