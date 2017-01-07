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
	vector<Color> lightRadiances;
	vector<Vector4> lightDirs;
	vector<double> distL2Ps;

	for(light = lights.cbegin(); light != lights.cend(); ++light) {

		Color illuminated(0, 0, 0);

		(*light)->Illuminate(p, lightDirs, lightRadiances, distL2Ps);

		// local illumination calculation for all light samples
		for (size_t lightSample = 0; lightSample != lightDirs.size(); ++lightSample) {
			Color lightRadiance = lightRadiances[lightSample];
			Vector4 lightDir = lightDirs[lightSample];
			double distL2P = distL2Ps[lightSample];

			// ambient
			if (useAmbient) {
				illuminated += lightRadiance * object->GetAmbient();
			}

			// shadow ray test
			bool visible = !Assistant::Shadow(Ray(p, -lightDir), objects, distL2P);

			// apply diffuse and specular effect if not shadow found
			if (visible) {
				// diffuse 
				diffuseScale = std::max(0.0, N * (-lightDir));
				illuminated += lightRadiance * object->GetDiffuse() * diffuseScale;

				// specular
				Vector4 H = (-lightDir + V);
				H.Normalize();
				specularScale = std::max(0.0, N * H);
				specularScale = pow(specularScale, object->GetSpecularExp());
				illuminated += lightRadiance * object->GetSpecular() * specularScale;
			}
		}
		
		// average samples
		if (!lightDirs.empty()) {
			ret += illuminated / (double)lightDirs.size();
		}
	}

	return ret;
}