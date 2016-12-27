#ifndef IMAGE_H
#define IMAGE_H

#include <vector>
#include <cmath>
#include "Color.h"

using namespace std;

class Image
{
public:
	Image();
	Image(const vector< Color >&, size_t width, size_t height);
	Image(Image &&);

	Color Variance(size_t left, size_t up, size_t right, size_t down) const;

	size_t GetWidth(void) const;
	size_t GetHeight(void) const;

	Color GetPixel(size_t x, size_t y) const;

	static void Create(string filename,
		const unique_ptr<Color[]> &image,
		const uint16_t width,
		const uint16_t height);

	Image& operator=(Image &&);

private:
	unique_ptr<Color[]> pixels;
	size_t Width;
	size_t Height;

	void Release(void);
};

#endif