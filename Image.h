#ifndef NEWIMAGE_H
#define NEWIMAGE_H

#include "lw_image\lw_image.h"
#include "Color.h"

class Image
{
public:
	Image();
	Image(size_t width, size_t height);
	Image(const Image &image);
	// Image(Image &&image);

	virtual ~Image();

	Image& operator=(const Image &image);
	// Image& operator=(Image &&image);

	static Image FromFile(const std::string &filename);

	const Color operator()(size_t x, size_t y) const;
	//Color& Pixel(size_t x, size_t y);
	void SetPixel(size_t x, size_t y, const Color &);

	void Resize(const size_t width, const size_t height);
	void Fill(const Color &color);

	virtual void Load(const std::string &filename);
	virtual void Save(const std::string &filename) const;

	inline size_t Width() const { return baseImage.width(); }
	inline size_t Height() const { return baseImage.height(); }

protected:
	lw::Image baseImage;
};

#endif