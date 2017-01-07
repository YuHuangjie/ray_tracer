#include "Camera.h"
#include <cassert>
#include <cmath>
#include <random>
#include "Define.h"

#define PI 3.141592654

Camera::Camera()
	: up(0, 1, 0), lookat(0, 0, 1), eye(0, 0, 0),
	width(320), height(240), near(1), far(3),
	aspect(320.0 / 240), theta(PI / 2), aperture_r(0)

{
	aspect = 1.0 * width / height;
	theta = PI / 2;
	UpdateGeo();
}

Camera::Camera(Point4 pos)
	: up(0, 1, 0), lookat(0, 0, 1), eye(pos),
	width(320), height(240), near(1), far(3),
	aspect((double)width / height), theta(PI / 2), aperture_r(0)
{
	aspect = 1.0 * width / height;
	theta = PI / 2; 
	UpdateGeo();
}

Camera::Camera(Point4 pos, Vector4 up, Vector4 lookat)
	: eye(pos),
	width(320), height(240), near(1), far(3),
	aspect((double)width / height), theta(PI / 2), aperture_r(0)
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

/* depth of field related */
void Camera::SetAperture(const double f, const double r, const uint16_t num)
{
	SetFocalLength(f);
	SetApertureRadius(r);
	aperture_samples = SampleAperture((num == 0 ? 16 : num));
}

void Camera::SetFocalLength(const double f) {
	this->f = (f <= near ? near + 1 : f);
}

void Camera::SetApertureRadius(const double r) {
	aperture_r = (r <= 0 ? 0 : r);
}


void Camera::SetApertureSampleNum(const uint16_t num) {
	aperture_samples = SampleAperture(num);
}

vector<Point4> Camera::SampleAperture(uint16_t num_sample) const {
	vector<Point4> samples;
	double fracW, fracH;

	// generate random distribution
	random_device rd;
	mt19937 mt(rd());
	double max = mt.max();

	samples.reserve(num_sample);

	// generate random sample point inside aperture
	for (int i = 0; i != num_sample;) {
		fracW = mt() * 2 / max - 1;
		fracH = mt() * 2 / max - 1;
		if (fracW * fracW + fracH * fracH <= 1.0) {
			//samples.push_back(eye);
			samples.push_back(eye + dirU * fracW * aperture_r + dirV * fracH * aperture_r);
			++i;
		}
	}
	return samples;
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

	ret.reserve(sample * aperture_samples.size());

	// center of the image plane
	Point4 image_plane_center = eye - lookat * near;
	// center of the focal plane
	Point4 focal_plane_center = eye - lookat * f;

	// generate random sample point inside a pixel
	for (int i = 0; i != sample; ++i) {

		fracW = 1.0 * (u + mt() / max) / width * w;
		fracH = 1.0 * (v + mt() / max) / height * h;
		Point4 pos = corner + dirU * fracW + dirV * fracH;	// pixel position
		// pinhole camera
		if (aperture_r == 0) {
			ray.SetOrigin(eye);
			ray.SetDirection(pos - eye);
			ret.push_back(ray);
		}
		// camera with depth of field
		else {
			for (unsigned int j = 0; j < aperture_samples.size(); ++j) {
				// aperture sample point
				const Point4 &aperture_p = aperture_samples[j];
				// the point on focal plane that the ray must go through
				Point4 focal_plane_pos = focal_plane_center + (pos - image_plane_center) * f / near;
				ray.SetOrigin(aperture_p);
				ray.SetDirection(focal_plane_pos - aperture_p);
				ret.push_back(ray);
			}
		}
	}

	return ret;
}