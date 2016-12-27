#include "Image.h"
#include <fstream>

Image::Image()
{}

Image::Image(
	const vector< Color > &pixels,
	size_t width, 
	size_t height)
	: Width(width), Height(height)
{
	this->pixels = make_unique<Color[]>(Width * Height);

	// fill image with pixels
	for (int p = 0; p != Width * Height; ++p) {
		this->pixels[p] = pixels[p];
	}
}

Image::Image(Image &&image)
	:Image()
{
	this->operator=(move(image));
}

Color Image::Variance(size_t left, size_t up, size_t right, size_t down) const
{
	Color mean(0, 0, 0);

	// calculate image mean
	for (size_t x = left; x != right; ++x) {
		for (size_t y = up; y != down; ++y) {
			size_t index = y * Width + x;	// linear index of pixel (u, v)
			mean += pixels[index];
		}
	}
	mean = mean / ((right - left) * (down - up));

	Color var(0, 0, 0);
	Color c;
	// calculate image variance
	for (size_t x = left; x != right; ++x) {
		for (size_t y = up; y != down; ++y) {
			c = pixels[y * Width + x];
			var += (c - mean) * (c - mean);
		}
	}
	var = var / ((right - left) * (down - up));
	var = Color(sqrt(var.R), sqrt(var.G), sqrt(var.B));

	return var;
}

size_t Image::GetWidth(void) const
{
	return Width;
}

size_t Image::GetHeight(void) const
{
	return Height;
}

Color Image::GetPixel(size_t x, size_t y) const
{
	return pixels[y * Width + x];
}

Image& Image::operator=(Image &&image)
{
	Release();

	Width = image.Width;
	Height = image.Height;
	pixels = move(image.pixels);

	image.Width = 0;
	image.Height = 0;

	return *this;
}

void Image::Release(void)
{
	Width = 0;
	Height = 0;
	pixels.reset();
}

void Image::Create(string filename,
	const unique_ptr<Color[]> &image,
	const uint16_t width,
	const uint16_t height)
{
	if (filename.empty() || width <= 0 || height <= 0 || image == nullptr) {
		return;
	}

	// correct file extension if needed
	if (filename.size() < 5) {
		filename += ".ppm";
	}
	else if (filename.substr(filename.size() - 4, 4) != ".ppm") {
		filename += ".ppm";
	}

	// write to buffer
	char *data = new char[width * height * 3];
	Color8 clampedColor;
	Color ref = Color::Threshold(image, width * height);

	for (int h = 0; h != height; ++h) {
		for (int w = 0; w != width; ++w) {
			//clampedColor = image[w * width + h].Clamp();
			clampedColor = image[w * width + h].Squeeze(ref);
			data[3 * (h*width + w)] = clampedColor.R;
			data[3 * (h*width + w) + 1] = clampedColor.G;
			data[3 * (h*width + w) + 2] = clampedColor.B;
		}
	}

	ofstream of(filename, ios_base::binary);
	of << "P6" << ' ' << width << ' ' << height << ' ' << 255 << '\n';
	of.write(data, width * height * 3);

	of.close();
}