#include <vector>

#include "Camera.h"
#include "InfinitePlane.h"
#include "Sphere.h"
#include "PointLight.h"
#include "Renderer.h"
#include "Image.h"

int main()
{
	Camera camera(Point4(4, 2, 2), Vector4(0, 0, 1), Vector4(1, 0, 0));
	Renderer renderer(2);
	vector< shared_ptr<const Primitive> > objects;
	vector< shared_ptr<const Light> > lights;

	// set camera resolution
	camera.SetResolution(640, 480);

	shared_ptr<Sphere> sphere1 = make_shared<Sphere>(Point4(1, 0, 2), 1);
	shared_ptr<Sphere> sphere2 = make_shared<Sphere>(Point4(1, 4, 2), 1);
	shared_ptr<InfinitePlane> bot = make_shared<InfinitePlane>(Point4(0, 0, 0), Vector4(0, 0, 1));
	shared_ptr<InfinitePlane> left = make_shared<InfinitePlane>(Point4(0, -2, 0), Vector4(0, 1, 0));
	shared_ptr<InfinitePlane> back = make_shared<InfinitePlane>(Point4(-1, 0, 0), Vector4(1, 0, 0));
	shared_ptr<InfinitePlane> right = make_shared<InfinitePlane>(Point4(0, 6, 0), Vector4(0, -1, 0));
	shared_ptr<InfinitePlane> up = make_shared<InfinitePlane>(Point4(0, 0, 4), Vector4(0, 0, -1));

	sphere1->SetAmbient(Color(0.1, 0.1, 0.1));
	sphere1->SetDiffuse(Color(0.5, 0.5, 0.0));
	sphere1->SetSpecular(Color(0.4, 0.4, 0.0));
	sphere1->SetSpecularExp(60);
	sphere1->SetReflection(0.5);
	objects.push_back(sphere1);

	sphere2->SetAmbient(Color(0.1, 0.1, 0.1));
	sphere2->SetDiffuse(Color(0.0, 0.5, 0.5));
	sphere2->SetSpecular(Color(0.0, 0.4, 0.4));
	sphere2->SetSpecularExp(20);
	sphere2->SetReflection(0.5);
	objects.push_back(sphere2);

	bot->SetAmbient(Color(0.3, 0.4, 0.6));
	bot->SetDiffuse(Color(0.3, 0.4, 0.6));
	bot->SetSpecular(Color(0, 0, 0));
	bot->SetSpecularExp(128);
	bot->SetReflection(0.5);
	objects.push_back(bot);

	left->SetAmbient(Color(0.3, 0.8, 0.6));
	left->SetDiffuse(Color(0.3, 0.8, 0.6));
	left->SetSpecular(Color(0, 0, 0));
	left->SetSpecularExp(128);
	left->SetReflection(0.5);
	objects.push_back(left);

	back->SetAmbient(Color(0.8, 0.4, 0.6));
	back->SetDiffuse(Color(0.8, 0.4, 0.6));
	back->SetSpecular(Color(0, 0, 0));
	back->SetSpecularExp(128);
	back->SetReflection(0.5);
	objects.push_back(back);

	right->SetAmbient(Color(0.3, 0.8, 0.6));
	right->SetDiffuse(Color(0.3, 0.8, 0.6));
	right->SetSpecular(Color(0, 0, 0));
	right->SetSpecularExp(128);
	right->SetReflection(0.5);
	objects.push_back(right);

	up->SetAmbient(Color(0.3, 0.4, 0.6));
	up->SetDiffuse(Color(0.3, 0.4, 0.6));
	up->SetSpecular(Color(0, 0, 0));
	up->SetSpecularExp(128);
	up->SetReflection(0.5);
	objects.push_back(up);


	shared_ptr<PointLight> pl = make_shared<PointLight>(camera.GetEye());
	pl->SetAmbient(Color(0.1, 0.1, 0.1));
	pl->SetDiffuse(Color(0.7, 0.7, 0.7));
	pl->SetSpecular(Color(0.7, 0.7, 0.7));
	pl->SetIntensity(40);
	lights.push_back(pl);

	vector<Ray> rays;
	uint16_t W = camera.GetWidth();
	uint16_t H = camera.GetHeight();
	Image photo(W, H);
	int sample = 8;

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
	}

	photo.Save("result/photo.png");






	//Image::Create("a.ppm", image, W, H);

	//vector< shared_ptr<Mesh> > meshes;
	//shared_ptr<Mesh> mesh = make_shared<Mesh>("mesh\\bunny.obj");
	//meshes.push_back(mesh);

	//vector<Ray> rays;
	//vector<Ray>::const_iterator it;
	//Intersection inters;
	//uint16_t W = camera.GetWidth();
	//uint16_t H = camera.GetHeight();
	////Color **image = new Color*[W];
	//unique_ptr<Color[]> image = make_unique<Color[]>(W * H);
	//int sample = 8;

	//unique_ptr<CubeMap> cubeMap = CubeMap::Build("hdr\\skybox.png", Color(0.2, 0.2, 0.2));
	//cubeMap->BuildBBox(Point4(0, 0, 0), 20);
	//cubeMap->BuildSample(5);
	//cout << cubeMap->samples.size() << endl;

	//lw::Image lwImage(1280, 720);

	//// reset image
	////for (int i = 0; i != W; ++i) {
	////	image[i] = new Color[H];
	////}

	//// render
	//for (int w = 0; w != camera.GetWidth(); ++w) {
	//	for (int h = 0; h != camera.GetHeight(); ++h) {
	//		rays = camera.GeneratePrimaryRay(w, h, sample);
	//		Color &color = image[h * camera.GetWidth() + w];

	//		for (it = rays.cbegin(); it != rays.cend(); ++it) {
	//			//color += renderer.RenderRay(*it, vector<Primitive*>(), meshes, lights);
	//			
	//			color += renderer.RenderRayEnv(*it, meshes, cubeMap);

	//			if (w == 147 && h == 62) {
	//				int a = 1;
	//			}
	//		}
	//		color = color * (1.0 / sample);

	//		lwImage.pixel(w, h) = lw::Color(color.R, color.G, color.B);
	//	}

	//	cout << "(" << w << ")" << endl;
	//	// show progress
	//}

	//lwImage.save("a.png");
	//Image::Create("a.ppm", image, camera.GetWidth(), camera.GetHeight());
}