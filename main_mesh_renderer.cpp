#include <vector>

#include "Camera.h"
#include "InfinitePlane.h"
#include "Sphere.h"
#include "PointLight.h"
#include "Renderer.h"
#include "Image.h"
#include "mesh.h"

int main()
{
	Camera camera(Point4(0, 5, 8), Vector4(0, 1, 0), Vector4(0, 0, 1));
	Renderer renderer(2);
	vector< shared_ptr<const Primitive> > objects;
	vector< shared_ptr<const Light> > lights;

	// set camera resolution
	camera.SetResolution(640, 480);

	shared_ptr<InfinitePlane> left = make_shared<InfinitePlane>(Point4(-8, 0, 0), Vector4(1, 0, 0));
	shared_ptr<InfinitePlane> right = make_shared<InfinitePlane>(Point4(8, 0, 0), Vector4(-1, 0, 0));
	shared_ptr<InfinitePlane> top = make_shared<InfinitePlane>(Point4(0, 10, 0), Vector4(0, -1, 0));
	shared_ptr<InfinitePlane> bot = make_shared<InfinitePlane>(Point4(0, -1, 0), Vector4(0, 1, 0));
	shared_ptr<InfinitePlane> back = make_shared<InfinitePlane>(Point4(0, 0, -2), Vector4(0, 0, 1));
	shared_ptr<Mesh> dragon = make_shared<Mesh>("mesh_resource/dragon2.obj");

	// bottom plane
	bot->SetAmbient(Color(0.3, 0.4, 0.6));
	bot->SetDiffuse(Color(0.3, 0.4, 0.6));
	bot->SetSpecular(Color(0, 0, 0));
	bot->SetSpecularExp(128);
	bot->SetReflection(0.5);
	bot->SetRoughness(0.8);
	objects.push_back(bot);

	// left plane
	left->SetAmbient(Color(0.3, 0.8, 0.6));
	left->SetDiffuse(Color(0.3, 0.8, 0.6));
	left->SetSpecular(Color(0, 0, 0));
	left->SetSpecularExp(128);
	left->SetReflection(0.5);
	left->SetRoughness(0.8);
	objects.push_back(left);

	// back plane
	back->SetAmbient(Color(0.8, 0.4, 0.6));
	back->SetDiffuse(Color(0.8, 0.4, 0.6));
	back->SetSpecular(Color(0, 0, 0));
	back->SetSpecularExp(128);
	back->SetReflection(0.5);
	back->SetRoughness(0.8);
	objects.push_back(back);

	// right plane
	right->SetAmbient(Color(0.3, 0.8, 0.6));
	right->SetDiffuse(Color(0.3, 0.8, 0.6));
	right->SetSpecular(Color(0, 0, 0));
	right->SetSpecularExp(128);
	right->SetReflection(0.5);
	right->SetRoughness(0.8);
	objects.push_back(right);

	// top plane
	top->SetAmbient(Color(0.3, 0.4, 0.6));
	top->SetDiffuse(Color(0.3, 0.4, 0.6));
	top->SetSpecular(Color(0, 0, 0));
	top->SetSpecularExp(128);
	top->SetReflection(0.5);
	top->SetRoughness(0.8);
	objects.push_back(top);

	// add a .obj 
	dragon->SetRoughness(0.1);
	objects.push_back(dragon);

	// add a point light
	shared_ptr<PointLight> pl = make_shared<PointLight>(camera.GetEye());
	pl->SetIntensity(Color(70, 70, 70));
	lights.push_back(pl);

	/* main render procedure */
	vector<Ray> rays;
	uint16_t W = camera.GetWidth();
	uint16_t H = camera.GetHeight();
	Image photo(W, H);
	int sample = 16;

	// sample pixels and render each ray
	for (int w = 0; w != camera.GetWidth(); ++w) {
		for (int h = 0; h != camera.GetHeight(); ++h) {
			vector<Ray>::const_iterator it;
			Color color = Color(0, 0, 0);
			rays = camera.GeneratePrimaryRay(w, h, sample);

			for (it = rays.cbegin(); it != rays.cend(); ++it) {
				color += renderer.RenderRay(*it, objects, lights);
			}

			color = color * (1.0 / sample);
			photo.SetPixel(w, h, color);
		}
		cout << w << endl;
	}

	photo.Save("result/cook_torrance_mesh_render.png");

	return 0;
}