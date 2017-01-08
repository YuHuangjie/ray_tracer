#include <vector>
#include <thread>
#include <ctime>

#include "Camera.h"
#include "InfinitePlane.h"
#include "Sphere.h"
#include "PointLight.h"
#include "AreaLight.h"
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
	Camera camera(Point4(6, 2, 1), Vector4(0, 0, 1), Vector4(1, 0, 0));
	Renderer renderer(2);
	renderer.SetIndirectDiffuseSample(128);
	renderer.SetIndirectSpecularSample(64);
	vector< shared_ptr<const Primitive> > objects;
	vector< shared_ptr<const Light> > lights;

	// set camera resolution
	camera.SetResolution(640, 480);
	// set camera aperture
	camera.SetAperture(2, 0.3, 100);

	// declare analytic objects
	shared_ptr<Sphere> sphere1 = make_shared<Sphere>(Point4(3.5, 1, 0.55), 0.5);
	shared_ptr<Sphere> sphere2 = make_shared<Sphere>(Point4(1, 3.5, 1), 1);
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
	sphere1->SetRoughness(0.8);
	objects.push_back(sphere1);

	sphere2->SetAmbient(Color(0.1, 0.1, 0.1));
	sphere2->SetDiffuse(Color(0.4, 0.4, 0.4));
	sphere2->SetSpecular(Color(0.6, 0.6, 0.6));
	sphere2->SetSpecularExp(20);
	sphere2->SetReflection(0.5);
	sphere2->SetRoughness(0.2);
	objects.push_back(sphere2);

	bot->SetAmbient(Color(0.1, 0.1, 0.1));
	bot->SetDiffuse(Color(0.1, 0.1, 0.7));
	bot->SetSpecular(Color(0.1, 0.1, 0.3));
	bot->SetSpecularExp(128);
	bot->SetReflection(0.5);
	bot->SetRoughness(0.7);
	objects.push_back(bot);

	left->SetAmbient(Color(0.1, 0.1, 0.1));
	//left->SetDiffuse(Color(0.1, 0.3, 0.1));
	//left->SetSpecular(Color(0.1, 0.7, 0.1));
	left->SetDiffuse(Color(0.5, 0.5, 0.5));
	left->SetSpecular(Color(0.5, 0.5, 0.5));
	left->SetSpecularExp(128);
	left->SetReflection(0.5);
	left->SetRoughness(0.1);
	objects.push_back(left);

	back->SetAmbient(Color(0.1, 0.1, 0.1));
	/*back->SetDiffuse(Color(0.8, 0.1, 0.1));
	back->SetSpecular(Color(0.2, 0.1, 0.1));*/
	back->SetDiffuse(Color(0.5, 0.5, 0.5));
	back->SetSpecular(Color(0.5, 0.5, 0.5));
	back->SetSpecularExp(128);
	back->SetReflection(0.5);
	back->SetRoughness(0.5);
	objects.push_back(back);

	right->SetAmbient(Color(0.1, 0.1, 0.1));
	//right->SetDiffuse(Color(0.1, 0.3, 0.1));
	//right->SetSpecular(Color(0.1, 0.7, 0.1));
	right->SetDiffuse(Color(0.5, 0.5, 0.5));
	right->SetSpecular(Color(0.5, 0.5, 0.5)); 
	right->SetSpecularExp(128);
	right->SetReflection(0.5);
	right->SetRoughness(0.1);
	objects.push_back(right);

	up->SetAmbient(Color(0.1, 0.1, 0.1));
	up->SetDiffuse(Color(0.5, 0.5, 0.5));
	up->SetSpecular(Color(0.5, 0.5, 0.5));
	up->SetSpecularExp(128);
	up->SetReflection(0.5);
	up->SetRoughness(0.5);
	objects.push_back(up);

	// declare point light
	//shared_ptr<PointLight> pl = make_shared<PointLight>(Point4(1, 2, 3.9));
	//pl->SetIntensity(Color(70, 70, 70));
	//lights.push_back(pl);
	shared_ptr<AreaLight> al = make_shared<AreaLight>(Point4(3, 2, 3), Vector4(0, 0, -1), 1, 1);
	al->SetIntensity(Color(70, 70, 70));
	al->SetSample(6, 6);
	lights.push_back(al);

	// set variables
	vector<Ray> rays;
	uint16_t W = camera.GetWidth();
	uint16_t H = camera.GetHeight();
	Image photo(W, H);
	int sample = 8;
	bool indirectLighting = true;
	uint16_t maxThreadCount = 8;
	Color tc[16];
	thread t[16];

	// set timer
	clock_t start = clock();

	// render main process
	for (uint16_t w = 0; w != camera.GetWidth(); w++) {
		for (uint16_t h = 0; h < camera.GetHeight(); h += maxThreadCount) {
			uint16_t threadCount = (camera.GetHeight() - h < maxThreadCount) ? camera.GetHeight() - h : maxThreadCount;
			// multithread acceleration
			for (uint16_t i = 0; i != threadCount; ++i) {
				t[i] = thread(OnRenderColumn, camera, ref(renderer), objects, lights, w, h + i, sample, indirectLighting, ref(tc[i]));
			}
			for (uint16_t i = 0; i != threadCount; ++i) {
				t[i].join();
			}
			for (uint16_t i = 0; i != threadCount; ++i) {
				photo.SetPixel(w, h + i, tc[i]);
			}
		}

		cout << ((double)w / camera.GetWidth()) * 100 << "%" << endl;
	}

	// output rendering time
	uint32_t sec = (clock() - start) / CLOCKS_PER_SEC;
	uint32_t min = sec / 60;
	uint32_t hour = min / 60;
	min = min - hour * 60;
	sec = sec - hour * 3600 - min * 60;
	cout << "render time: " << hour << "h " << min << "m " << sec << "s" << endl;

	// saving image
	string  filename = "result/temp.png";
	cout << "saving to " << filename << endl;
	photo.Save(filename);

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
	if (h >= camera.GetHeight()) {
		ret = Color(0, 0, 0);
		return;
	}

	ret = Color(0, 0, 0);

	vector<Ray>::const_iterator it;
	vector<Ray> rays = camera.GeneratePrimaryRay(w, h, sample);

	for (it = rays.cbegin(); it != rays.cend(); ++it) {
		ret += renderer.RenderRay(*it, objects, lights, indirect);
	}

	ret = ret * (1.0 / rays.size());
}
