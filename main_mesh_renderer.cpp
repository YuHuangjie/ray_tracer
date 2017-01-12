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
#include "mesh.h"

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
	Camera camera(Point4(3, 1, 4), Vector4(0, 1, 0), Vector4(0, 0, 1));
	Renderer renderer(2);
	vector< shared_ptr<const Primitive> > objects;
	vector< shared_ptr<const Light> > lights;

	// set camera resolution
	camera.SetResolution(640, 480);
	// set camera aperture
	camera.SetAperture(5.5, 1.5, 6000);

	// set indirect lighting samples
	renderer.SetIndirectDiffuseSample(128);
	renderer.SetIndirectSpecularSample(64);

	// analytic objects
	//shared_ptr<InfinitePlane> left = make_shared<InfinitePlane>(Point4(-8, 0, 0), Vector4(1, 0, 0));
	//shared_ptr<InfinitePlane> right = make_shared<InfinitePlane>(Point4(8, 0, 0), Vector4(-1, 0, 0));
	//shared_ptr<InfinitePlane> top = make_shared<InfinitePlane>(Point4(0, 10, 0), Vector4(0, -1, 0));
	//shared_ptr<InfinitePlane> bot = make_shared<InfinitePlane>(Point4(0, -1, 0), Vector4(0, 1, 0));
	shared_ptr<InfinitePlane> ground = make_shared<InfinitePlane>(Point4(0, -1.2, 0), Vector4(0, 1, 0));
	// mesh objects
	shared_ptr<Mesh> bunny_front = make_shared<Mesh>("mesh_resource/bunny_front_2.obj");
	shared_ptr<Mesh> bunny_back = make_shared<Mesh>("mesh_resource/bunny_back.obj");
	shared_ptr<Mesh> bunny_last = make_shared<Mesh>("mesh_resource/bunny_last.obj");


	//// bottom plane
	//bot->SetAmbient(Color(0.3, 0.4, 0.6));
	//bot->SetDiffuse(Color(0.3, 0.4, 0.6));
	//bot->SetSpecular(Color(0, 0, 0));
	//bot->SetSpecularExp(128);
	//bot->SetReflection(0.5);
	//bot->SetRoughness(0.8);
	//objects.push_ground(bot);

	//// left plane
	//left->SetAmbient(Color(0.3, 0.8, 0.6));
	//left->SetDiffuse(Color(0.3, 0.8, 0.6));
	//left->SetSpecular(Color(0, 0, 0));
	//left->SetSpecularExp(128);
	//left->SetReflection(0.5);
	//left->SetRoughness(0.8);
	//objects.push_ground(left);

	// ground plane
	ground->SetAmbient(Color(0.1, 0.1, 0.1));
	ground->SetDiffuse(Color(0.98, 0.98, 0.98));
	ground->SetSpecular(Color(0, 0, 0));
	ground->SetSpecularExp(128);
	ground->SetReflection(0.5);
	ground->SetRoughness(0.8);
	objects.push_back(ground);

	//// right plane
	//right->SetAmbient(Color(0.3, 0.8, 0.6));
	//right->SetDiffuse(Color(0.3, 0.8, 0.6));
	//right->SetSpecular(Color(0, 0, 0));
	//right->SetSpecularExp(128);
	//right->SetReflection(0.5);
	//right->SetRoughness(0.8);
	//objects.push_ground(right);

	//// top plane
	//top->SetAmbient(Color(0.3, 0.4, 0.6));
	//top->SetDiffuse(Color(0.3, 0.4, 0.6));
	//top->SetSpecular(Color(0, 0, 0));
	//top->SetSpecularExp(128);
	//top->SetReflection(0.5);
	//top->SetRoughness(0.8);
	//objects.push_ground(top);

	// set mesh property
	bunny_front->SetRoughness(0.2);
	bunny_back->SetRoughness(0.8);
	bunny_last->SetRoughness(0.5);
	objects.push_back(bunny_front);
	objects.push_back(bunny_back);
	objects.push_back(bunny_last);

	 //add an area light
	//shared_ptr<AreaLight> al = make_shared<AreaLight>(Point4(5, 3, 0), Vector4(0, -1, 0), 4, 4);
	//al->SetIntensity(Color(60, 60, 60));
	//al->SetSample(10, 10);
	//shared_ptr<PointLight> pl = make_shared<PointLight>(Point4(2, 2, 0));
	//pl->SetIntensity(Color(40, 40, 40));
	//lights.push_back(al);
	//lights.push_back(pl);

	shared_ptr<PointLight> pl_1 = make_shared<PointLight>(Point4(8, 3, 1));
	pl_1->SetIntensity(Color(30, 30, 30));
	lights.push_back(pl_1);
	shared_ptr<PointLight> pl_2 = make_shared<PointLight>(Point4(2, 3, 5));
	pl_2->SetIntensity(Color(30, 30, 30));
	lights.push_back(pl_2);
	shared_ptr<PointLight> pl_3 = make_shared<PointLight>(Point4(-2, 3, -2));
	pl_3->SetIntensity(Color(40, 40, 40));
	lights.push_back(pl_3);
	shared_ptr<PointLight> pl_4 = make_shared<PointLight>(Point4(4, 3, -5));
	pl_4->SetIntensity(Color(40, 40, 40));
	lights.push_back(pl_4);

	/* main render procedure */
	// set variables
	vector<Ray> rays;
	uint16_t W = camera.GetWidth();
	uint16_t H = camera.GetHeight();
	Image photo(W, H);
	int sample = 3;
	bool indirectLighting = false;
	uint16_t maxThreadCount = 7;
	Color tc[32];
	thread t[32];

	// set timer
	clock_t start = clock();

	// main loop
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
	
	return 0;
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