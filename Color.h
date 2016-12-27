#ifndef COLOR_H
#define COLOR_H

#include <cstdint>
#include <iostream>
#include <memory>
#include "lw_image\lw_color.h"

using namespace std;

class Color8
{
public:
	Color8();
	Color8(uint8_t R, uint8_t G, uint8_t B);

	uint8_t R;
	uint8_t G;
	uint8_t B;

	friend ostream& operator<<(ostream &os, const Color8 &);
};

class Color
{
public:
	Color();
	Color(double R, double G, double B);
	explicit Color(const lw::Color);

	Color operator-(const Color &) const;
	Color operator+(const Color &) const;
	Color operator*(const double s) const;
	Color operator*(const Color &) const;
	Color operator/(const double s) const;
	Color& operator+=(const Color &);
	bool operator<(const Color &) const;

	Color Clamp(void);
	Color8 Squeeze(const Color &ref);
	static Color Threshold(const unique_ptr<Color[]> &, size_t size);

	friend ostream& operator<<(ostream&, const Color&);

public:
	double R;
	double G;
	double B;
};


#endif