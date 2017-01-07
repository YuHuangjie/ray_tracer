#ifndef CAMERA_H
#define CAMERA_H

#include "Vector4.h"
#include "Point4.h"
#include "Ray.h"
#include <cstdint>
#include <iostream>
#include <vector>

using namespace std;


class Camera
{
public:
	Camera();
	Camera(Point4 pos);
	Camera(Point4 pos, Vector4 up, Vector4 lookat);

	void SetGeo(const Vector4& up, const Vector4& lookat);
	Vector4 GetUp(void) const;
	Vector4 GetLookat(void) const;

	void SetEye(const Point4&);
	Point4 GetEye(void) const;

	void SetFoVD(const double theta);
	double GetFoVD(void) const;

	void SetAspect(const double aspect);
	double GetAspect(void) const;

	void SetResolution(const uint16_t width, const uint16_t height);
	uint16_t GetWidth(void) const;
	uint16_t GetHeight(void) const;

	void SetSight(const double near, const double far);
	double GetNear(void) const;
	double GetFar(void) const;

	vector<Ray> GeneratePrimaryRay(uint16_t u, const uint16_t v, int sample = 1) const;

	friend ostream& operator<<(ostream& os, const Camera &);

private:
	void UpdateGeo();

	Vector4 up;
	Vector4 lookat;
	Point4 eye;
	double aspect;		// width / height
	double theta;		// vertical field of view
	uint16_t width;		// column pixel range
	uint16_t height;	// row pixel range
	double near;
	double far;

	// helper member for calculating ray
	double w, h;		// image plane coordinate range
	Point4 corner;		// upper left coordinate
	Vector4 dirU;		// unit vector along column axis
	Vector4 dirV;		// unit vector along row axis
};


#endif