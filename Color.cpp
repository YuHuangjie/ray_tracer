#include "Color.h"
#include <algorithm>

Color8::Color8()
	: R(0), G(0), B(0)
{}

Color8::Color8(uint8_t R, uint8_t G, uint8_t B)
	: R(R), G(G), B(B)
{}

ostream& operator<<(ostream &os, const Color8 &color)
{
	os << "R: " << (uint16_t)color.R << ", "
		<< "G: " << (uint16_t)color.G << ", "
		<< "B: " << (uint16_t)color.B << ", ";
	return os;
}


Color::Color()
	: R(0), G(0), B(0)
{}

Color::Color(double R, double G, double B)
{
	double r = std::max(0.0, R);
	double g = std::max(0.0, G);
	double b = std::max(0.0, B);
	this->R = r;
	this->G = g;
	this->B = b;
}

Color::Color(const lw::Color c)
{
	this->R = c.red();
	this->G = c.green();
	this->B = c.blue();
}

Color Color::operator-(const Color &color) const
{
	return Color(R - color.R, G - color.G, B - color.B);
}

Color Color::operator+(const Color &color) const
{
	return Color(R + color.R, G + color.G, B + color.B);
}

Color Color::operator*(const double s) const
{
	return Color(R*s, G*s, B*s);
}

Color Color::operator*(const Color &color) const
{
	return Color(R*color.R, G*color.G, B*color.B);
}

Color Color::operator/(const double s) const
{
	double mul_s = 1 / s;
	return operator*(mul_s);
}

Color& Color::operator+=(const Color &color)
{
	this->R += color.R;
	this->G += color.G;
	this->B += color.B;
	return *this;
}

bool Color::operator<(const Color &c) const
{
	return (R < c.R && G < c.G && B < c.B);
}

Color8 Color::Squeeze(const Color &ref)
{
	uint8_t r = static_cast<uint8_t>((R / ref.R) * 255);
	uint8_t g = static_cast<uint8_t>((G / ref.G) * 255);
	uint8_t b = static_cast<uint8_t>((B / ref.B) * 255);
	return Color8(r, g, b);
}

Color Color::Clamp(void)
{
	double r = std::min(R, 1.0);
	double g = std::min(G, 1.0);
	double b = std::min(B, 1.0);
	return Color(r, g, b);
}

Color Color::Threshold(const unique_ptr<Color[]> &image, size_t size)
{
	double tR = 0;
	double tG = 0;
	double tB = 0;

	for (size_t i = 0; i != size; ++i) {
		Color c = image[i];
		tR = c.R > tR ? c.R : tR;
		tG = c.G > tG ? c.G : tG;
		tB = c.B > tB ? c.B : tB;
	}

	return Color(tR, tG, tB);
}

ostream& operator<<(ostream &os, const Color &color)
{
	os << "R: " << color.R << ", "
		<< "G: " << color.G << ", "
		<< "B: " << color.B << ", ";
	return os;
}