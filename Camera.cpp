#include "Camera.h"
#include <cassert>
#include <cmath>
#include <random>
#include "Define.h"

#define PI 3.141592654

Camera::Camera()
	: up(0, 1, 0), lookat(0, 0, 1), eye(0, 0, 0),
	width(320), height(240), near(1), far(3),
	aspect(320.0 / 240), theta(PI / 2)

{
	aspect = 1.0 * width / height;
	theta = PI / 2;
	UpdateGeo();
}

Camera::Camera(Point4 pos)
	: up(0, 1, 0), lookat(0, 0, 1), eye(pos),
	width(320), height(240), near(1), far(3),
	aspect((double)width / height), theta(PI / 2)
{
	aspect = 1.0 * width / height;
	theta = PI / 2; 
	UpdateGeo();
}

Camera::Camera(Point4 pos, Vector4 up, Vector4 lookat)
	: eye(pos),
	width(320), height(240), near(1), far(3),
	aspect((double)width / height), theta(PI / 2)
{
	if (abs(up * lookat) > EPS) {
		this->up = Vector4(0, 1, 0);
		this->lookat = Vector4(0, 0, 1);
	}
	else {
		this->up = up.NormalizedVec();
		this->lookat = lookat.NormalizedVec();
	}
	aspect = 1.0 * width / height;
	theta = PI / 2; 
	UpdateGeo();
}

void Camera::SetGeo(const Vector4 &up, const Vector4 &lookat)
{
	if (abs(up*lookat) > EPS) {
		return;
	}

	this->up = up;
	this->up.Normalize();
	this->lookat = lookat;
	this->lookat.Normalize();
	UpdateGeo();
}

Vector4 Camera::GetUp(void) const
{
	return up;
}

Vector4 Camera::GetLookat(void) const
{
	return lookat;
}

void Camera::SetEye(const Point4 &eye)
{
	this->eye = eye;
	UpdateGeo();
}

Point4 Camera::GetEye(void) const
{
	return eye;
}

void Camera::SetFoVD(const double theta)
{
	this->theta = theta * PI / 180;
	UpdateGeo();
}

double Camera::GetFoVD(void) const
{
	return theta * 180 / PI;
}

void Camera::SetAspect(const double aspect)
{
	this->aspect = aspect;
	UpdateGeo();
}

double Camera::GetAspect(void) const
{
	return aspect;
}

void Camera::SetResolution(const uint16_t width, const uint16_t height)
{
	this->width		= width;
	this->height	= height;
	this->aspect	= (double)width / height;
	UpdateGeo();
}

uint16_t Camera::GetWidth(void) const
{
	return width;
}

uint16_t Camera::GetHeight(void) const
{
	return height;
}

void Camera::SetSight(const double near, const double far)
{
	this->near = near;
	this->far = far;
}

double Camera::GetNear(void) const
{
	return near;
}

double Camera::GetFar(void) const
{
	return far;
}

vector<Ray> Camera::GeneratePrimaryRay(uint16_t u, uint16_t v, int sample) const
{
	Assertion(u < width && v < height && sample > 0, "pixel out of bound");
	Assertion(sample > 0, "sample should be greater than 0");

	vector<Ray> ret;
	Ray ray;
	double fracW, fracH;

	// generate random distribution
	random_device rd;
	mt19937 mt(rd());
	double max = mt.max();

	ret.reserve(sample);

	// generate random sample point inside a pixel
	for (int i = 0; i != sample; ++i) {
		/*double fracU = 1.0 * (u + 0.5) / width * w;
		double fracV = 1.0 * (v + 0.5) / height * h;

		ret.SetOrigin(eye);
		ret.SetDirection((corner + dirU*fracU + dirV*fracV - eye).Normalize());*/
		fracW = 1.0 * (u + mt() / max) / width * w;
		fracH = 1.0 * (v + mt() / max) / height * h;
		ray.SetOrigin(eye);
		ray.SetDirection((corner + dirU * fracW + dirV * fracH - eye).NormalizedVec());
		ret.push_back(ray);
	}

	return ret;
}

void Camera::UpdateGeo(void)
{
	Point4 center = eye - lookat * near;
	dirU = up.Cross(lookat);
	dirV = -up;
	h = 2 * near * tan(theta / 2);
	w = h * aspect;

	corner = center - dirU * w / 2 - dirV * h / 2;
}


ostream& operator<<(ostream &os, const Camera &camera)
{
	os << "eye: " << camera.GetEye() << endl
		<< "up: " << camera.GetUp() << endl
		<< "lookat: " << camera.GetLookat() << endl
		<< "width: " << camera.GetWidth() << endl
		<< "height: " << camera.GetHeight() << endl
		<< "near: " << camera.GetNear() << endl
		<< "far: " << camera.GetFar() << endl
		<< "FoV: " << camera.GetFoVD() << " degree" << endl;

	return os;
}

