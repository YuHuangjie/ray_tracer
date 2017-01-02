#include <vector>
#include <thread>

#include "Camera.h"
#include "InfinitePlane.h"
#include "Sphere.h"
#include "PointLight.h"
#include "Renderer.h"
#include "Image.h"

void OnRenderColumn(
	const Camera &camera,
	Renderer &renderer,
	const vector< shared_ptr<const Primitive> > &objects,
	const vector< shared_ptr<const Light> > &lights,
	const uint16_t &w, const uint16_t &h,
	const uint16_t &sample,
	const bool &indirect,
	Color &ret);

int main()
{
	Camera camera(Point4(4, 2, 1), Vector4(0, 0, 1), Vector4(1, 0, 0));
	Renderer renderer(2);
	renderer.SetIndirectDiffuseSample(128);
	vector< shared_ptr<const Primitive> > objects;
	vector< shared_ptr<const Light> > lights;

	// set camera resolution
	camera.SetResolution(200, 200);

	shared_ptr<Sphere> sphere1 = make_shared<Sphere>(Point4(1, 0, 1), 1);
	shared_ptr<Sphere> sphere2 = make_shared<Sphere>(Point4(1, 4, 1), 1);
	shared_ptr<InfinitePlane> bot = make_shared<InfinitePlane>(Point4(0, 0, 0), Vector4(0, 0, 1));
	shared_ptr<InfinitePlane> left = make_shared<InfinitePlane>(Point4(0, -2, 0), Vector4(0, 1, 0));
	shared_ptr<InfinitePlane> back = make_shared<InfinitePlane>(Point4(-1, 0, 0), Vector4(1, 0, 0));
	shared_ptr<InfinitePlane> right = make_shared<InfinitePlane>(Point4(0, 6, 0), Vector4(0, -1, 0));
	shared_ptr<InfinitePlane> up = make_shared<InfinitePlane>(Point4(0, 0, 4), Vector4(0, 0, -1));

	sphere1->SetAmbient(Color(0.1, 0.1, 0.1));
	sphere1->SetDiffuse(Color(0.8, 0.8, 0.8));
	sphere1->SetSpecular(Color(0.2, 0.2, 0.2));
	sphere1->SetSpecularExp(60);
	sphere1->SetReflection(0.5);
	sphere1->SetRoughness(0.3);
	objects.push_back(sphere1);

	sphere2->SetAmbient(Color(0.1, 0.1, 0.1));
	sphere2->SetDiffuse(Color(0.8, 0.8, 0.8));
	sphere2->SetSpecular(Color(0.2, 0.2, 0.2));
	sphere2->SetSpecularExp(20);
	sphere2->SetReflection(0.5);
	sphere2->SetRoughness(0.7);
	objects.push_back(sphere2);

	bot->SetAmbient(Color(0.1, 0.1, 0.1));
	bot->SetDiffuse(Color(0.3, 0.4, 0.6));
	bot->SetSpecular(Color(0, 0, 0));
	bot->SetSpecularExp(128);
	bot->SetReflection(0.5);
	objects.push_back(bot);

	left->SetAmbient(Color(0.1, 0.1, 0.1));
	left->SetDiffuse(Color(0.3, 0.8, 0.6));
	left->SetSpecular(Color(0, 0, 0));
	left->SetSpecularExp(128);
	left->SetReflection(0.5);
	objects.push_back(left);

	back->SetAmbient(Color(0.1, 0.1, 0.1));
	back->SetDiffuse(Color(0.8, 0.4, 0.6));
	back->SetSpecular(Color(0, 0, 0));
	back->SetSpecularExp(128);
	back->SetReflection(0.5);
	objects.push_back(back);

	right->SetAmbient(Color(0.1, 0.1, 0.1));
	right->SetDiffuse(Color(0.3, 0.8, 0.6));
	right->SetSpecular(Color(0, 0, 0));
	right->SetSpecularExp(128);
	right->SetReflection(0.5);
	objects.push_back(right);

	up->SetAmbient(Color(0.1, 0.1, 0.1));
	up->SetDiffuse(Color(0.3, 0.4, 0.6));
	up->SetSpecular(Color(0, 0, 0));
	up->SetSpecularExp(128);
	up->SetReflection(0.5);
	objects.push_back(up);


	shared_ptr<PointLight> pl = make_shared<PointLight>(Point4(1, 2, 3.9));
	pl->SetIntensity(Color(70, 70, 70));
	lights.push_back(pl);

	vector<Ray> rays;
	uint16_t W = camera.GetWidth();
	uint16_t H = camera.GetHeight();
	Image photo(W, H);
	int sample = 8;
	bool indirectLighting = true;
	uint16_t thread_count = 8;
	Color tc[16];
	thread t[16];

	// sample pixels and render each ray
	for (uint16_t w = 0; w != camera.GetWidth(); w++) {
		for (uint16_t h = 0; h != camera.GetHeight(); h += thread_count) {
			// multithread rendering
			for (uint16_t i = 0; i != thread_count; ++i) {
				t[i] = thread(OnRenderColumn, camera, ref(renderer), objects, lights, w, h + i, sample, indirectLighting, ref(tc[i]));
			}
			for (uint16_t i = 0; i != thread_count; ++i) {
				t[i].join();
			}
			for (uint16_t i = 0; i != thread_count; ++i) {
				photo.SetPixel(w, h + i, tc[i]);
			}
		}

		cout << ((double)w / camera.GetWidth()) * 100 << "%" << endl;
	}

	photo.Save("result/indirect_diffuse_cooktorrance_ball.png");

}

void OnRenderColumn(
	const Camera &camera,
	Renderer &renderer,
	const vector< shared_ptr<const Primitive> > &objects,
	const vector< shared_ptr<const Light> > &lights,
	const uint16_t &w, const uint16_t &h,
	const uint16_t &sample,
	const bool &indirect,
	Color &ret)
{
	if (w >= camera.GetWidth()) {
		ret = Color(0, 0, 0);
		return;
	}

	vector<Ray>::const_iterator it;
	vector<Ray> rays = camera.GeneratePrimaryRay(w, h, sample);

	for (it = rays.cbegin(); it != rays.cend(); ++it) {
		ret += renderer.RenderRay(*it, objects, lights, indirect);
	}

	ret = ret * (1.0 / sample);
}