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
	const Point4 P = inter.GetPosition();
	const shared_ptr<const Primitive> object = inter.GetPrimitive();
	double roughnessValue = object->GetRoughness();
	double F0 = object->GetFresnelReflection();
	Color kd = object->GetDiffuse();
	Color ks = object->GetSpecular();

	// direct lighting
	vector< shared_ptr<const Light> >::const_iterator light;
	vector<Color> lightRadiances;
	vector<double> distL2Ps;
	vector<Vector4> Ls;

	for (light = lights.cbegin(); light != lights.cend(); ++light) {

		Color illuminated(0, 0, 0);

		(*light)->Illuminate(P, Ls, lightRadiances, distL2Ps);

		// local illumination calculation for all lightsamples
		for (size_t lightSample = 0; lightSample != Ls.size(); ++lightSample) {
			Color lightRadiance = lightRadiances[lightSample];
			Vector4 L = Ls[lightSample];
			double distL2P = distL2Ps[lightSample];

			// ambient
			if (useAmbient) {
				illuminated += lightRadiance * object->GetAmbient();
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

				illuminated += lightRadiance * NdotL * (kd + ks * specular);
			}
		}

		// average samples
		if (!lightRadiances.empty()) {
			ret += illuminated / (double)lightRadiances.size();
		}
	}

	return ret;
}