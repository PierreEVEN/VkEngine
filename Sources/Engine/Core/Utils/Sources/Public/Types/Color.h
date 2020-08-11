#pragma once

struct SColor;
struct SLinearColor;

inline constexpr uint8_t DEFAULT_COLOR_RED_VALUE = 0;
inline constexpr uint8_t DEFAULT_COLOR_GREEN_VALUE = 0;
inline constexpr uint8_t DEFAULT_COLOR_BLUE_VALUE = 0;
inline constexpr uint8_t DEFAULT_COLOR_ALPHA_VALUE = 255;

inline constexpr float DEFAULT_LINEAR_COLOR_RED_VALUE = 0.f;
inline constexpr float DEFAULT_LINEAR_COLOR_GREEN_VALUE = 0.f;
inline constexpr float DEFAULT_LINEAR_COLOR_BLUE_VALUE = 0.f;
inline constexpr float DEFAULT_LINEAR_COLOR_ALPHA_VALUE = 1.f;

template<typename T, const T& defaultRedValue, const T& defaultGreenValue, const T& defaultBlueValue, const T& defaultAlphaValue>
struct IColor
{
	static const T WHITE;
	static const T GRAY;
	static const T BLACK;
	static const T RED;
	static const T BLUE;
	static const T GREEN;
	static const T ALPHA;

	explicit IColor() : r(defaultRedValue), g(defaultGreenValue), b(defaultBlueValue), a(defaultAlphaValue) {}
	explicit IColor(const T& linearValue) : r(linearValue), g(linearValue), b(linearValue), a(defaultAlphaValue) {}
	explicit IColor(const T& linearValue, const T& alphaValue) : r(linearValue), g(linearValue), b(linearValue), a(alphaValue) {}
	explicit IColor(const T& red, const T& green, const T& blue) : r(red), g(green), b(blue), a(defaultAlphaValue) {}

	explicit IColor(const T& red, const T& green, const T& blue, const T& alpha) : r(red), g(green), b(blue), a(alpha) {}

	inline const bool operator==(const IColor<T, defaultRedValue, defaultGreenValue, defaultBlueValue, defaultAlphaValue>& other) const
	{
		return
			r == other.r &&
			g == other.g &&
			b == other.b &&
			a == other.a;
	}

	inline const bool operator!=(const IColor<T, defaultRedValue, defaultGreenValue, defaultBlueValue, defaultAlphaValue>& other) const
	{
		return
			r != other.r ||
			g != other.g ||
			b != other.b ||
			a != other.a;
	}

	union
	{
		struct { T r, g, b, a; };
		struct { T x, y, z, w; };
		T coords[4];
	};
};

struct SColor : public IColor<uint8_t, DEFAULT_COLOR_RED_VALUE, DEFAULT_COLOR_GREEN_VALUE, DEFAULT_COLOR_BLUE_VALUE, DEFAULT_COLOR_ALPHA_VALUE>
{
	using IColor::IColor;

	static const uint32_t redMask = 0x000000FF; // Red mask
	static const uint32_t greenMask = 0x0000FF00; // Green mask
	static const uint32_t blueMask = 0x00FF0000; // Blue mask
	static const uint32_t alphaMask = 0xFF000000; // Alpha mask

	static const SColor WHITE; // White
	static const SColor GRAY; // Gray
	static const SColor BLACK; // Black
	static const SColor RED; // Red
	static const SColor BLUE; // Blue
	static const SColor GREEN; // Green
	static const SColor ALPHA; // Transparent

	SColor(const SLinearColor& source);

	inline const uint32_t& AsInt() const { return r + (g << 8) + (b << 16) + (a << 24); }
	inline static SColor FromInt(const uint32_t& source) { return SColor(source & redMask, (source & greenMask) >> 8, (source & blueMask) >> 16, (source & alphaMask) >> 24); }
};

struct SLinearColor : public IColor<float, DEFAULT_LINEAR_COLOR_RED_VALUE, DEFAULT_LINEAR_COLOR_GREEN_VALUE, DEFAULT_LINEAR_COLOR_BLUE_VALUE, DEFAULT_LINEAR_COLOR_ALPHA_VALUE>
{
	using IColor::IColor;

	static const SLinearColor WHITE; // White
	static const SLinearColor GRAY; // Gray
	static const SLinearColor BLACK; // Black
	static const SLinearColor RED; // Red
	static const SLinearColor BLUE; // Blue
	static const SLinearColor GREEN; // Green
	static const SLinearColor ALPHA; // Transparent

	SLinearColor(const SColor& source);
};

inline const SColor SColor::WHITE = SColor(255, 255, 255);
inline const SColor SColor::GRAY = SColor(128);
inline const SColor SColor::BLACK = SColor(0);
inline const SColor SColor::RED = SColor(255, 0, 0);
inline const SColor SColor::GREEN = SColor(0, 255, 0);
inline const SColor SColor::BLUE = SColor(0, 0, 255);
inline const SColor SColor::ALPHA = SColor(0, 0);

inline const SLinearColor SLinearColor::WHITE = SLinearColor(1.f);
inline const SLinearColor SLinearColor::GRAY = SLinearColor(0.5f);
inline const SLinearColor SLinearColor::BLACK = SLinearColor(0.f);
inline const SLinearColor SLinearColor::RED = SLinearColor(1.f, 0.f, 0.f);
inline const SLinearColor SLinearColor::GREEN = SLinearColor(0.f, 1.f, 0.f);
inline const SLinearColor SLinearColor::BLUE = SLinearColor(0.f, 0.f, 1.f);
inline const SLinearColor SLinearColor::ALPHA = SLinearColor(0.f, 0.f);

inline SColor::SColor(const SLinearColor& source) : SColor((uint8_t)source.r * 255, (uint8_t)source.g * 255, (uint8_t)source.b * 255, (uint8_t)source.a * 255) {}
inline SLinearColor::SLinearColor(const SColor& source) : SLinearColor(source.r / 255.f, source.g / 255.f, source.b / 255.f, source.a / 255.f) {}