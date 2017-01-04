#ifdef _MSC_VER
#pragma once
#endif

#ifndef LW_IMAGE_H_
#define LW_IMAGE_H_

#include <string>
#include <memory>

#include "lw_color.h"

namespace lw {

	enum class Tonemap {
		Rainhard = 0x00
	};

	/** Image class.
	*/
	class  Image {
	protected:
		size_t _width/* = 0U*/;
		size_t _height/* = 0U*/;
		std::unique_ptr<Color[]> _pixels/* = {}*/;

	public:
		Image();
		Image(size_t width, size_t height);
		Image(const Image& image);
		Image(Image&& image);

		virtual ~Image();

		Image& operator=(const Image& image);
		Image& operator=(Image&& image);

		static Image fromFile(const std::string& filename);

		const Color& operator()(size_t x, size_t y) const;
		Color& pixel(size_t x, size_t y);

		void resize(const size_t width, const size_t height);
		void fill(const Color& color);

		/** Gamma correction.
		*  @param[in] gamma: gamma value
		*/
		void gammaCorrect(const double gamma);

		virtual void load(const std::string& filename);
		virtual void save(const std::string& filename) const;

		void tonemap(Tonemap algo = Tonemap::Rainhard);

		inline size_t width() const { return _width; }
		inline size_t height() const { return _height; }

	protected:
		void release();
		static double toReal(unsigned char b);
		static unsigned char toByte(double d);

		void loadBmp(const std::string& filename);
		void saveBmp(const std::string& filename) const;

		void loadHdr(const std::string& filename);
		void saveHdr(const std::string& filename) const;

		void loadPng(const std::string& filename);
		void savePng(const std::string& filename) const;
	};

}

#endif  // lw_IMAGE_H_
