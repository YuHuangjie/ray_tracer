#include "Image.h"

Image::Image()
{}

Image::Image(size_t width, size_t height)
	: baseImage(width, height)
{}

Image::Image(const Image &image)
	: baseImage(image.baseImage)
{}

Image::~Image()
{}

Image& Image::operator=(const Image& image)
{
	baseImage = image.baseImage;
	return *this;
}

Image Image::FromFile(const std::string &filename)
{
	Image img;
	img.baseImage.load(filename);
	return img;
}

const Color Image::operator()(size_t x, size_t y) const
{
	const lw::Color c = baseImage(x, y);
	return (Color)c;
}

void Image::SetPixel(size_t x, size_t y, const Color &color)
{
	baseImage.pixel(x, y) = lw::Color(color.R, color.G, color.B);
}

void Image::Resize(const size_t width, const size_t height)
{
	baseImage.resize(width, height);
}

void Image::Fill(const Color& color)
{
	lw::Color c(color.R, color.G, color.B);
	baseImage.fill(c);
}

void Image::Load(const std::string &filename)
{
	baseImage.load(filename);
}

void Image::Save(const std::string &filename) const
{
	baseImage.save(filename);
}