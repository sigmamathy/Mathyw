#pragma once

#include "./vector.hpp"

namespace Mathyw {

// Convert a RGBA hex code to RGBA vector4 (with range[0, 1])
// @param has_alpha: true if the hexcode contains alpha channel
constexpr Fvec4 Color(unsigned hex, bool has_alpha = false)
{
	return {
		(std::uint8_t)(hex >> (16 + 8 * has_alpha)) / 255.0f,
		(std::uint8_t)(hex >> (8 + 8 * has_alpha)) / 255.0f,
		(std::uint8_t)(hex >> (8 * has_alpha)) / 255.0f,
		has_alpha ? hex / 255.0f : 1.0f
	};
}

// Miscellaneous default colors
constexpr Fvec4 Black	= Color(0x000000);
constexpr Fvec4 White	= Color(0xFFFFFF);
constexpr Fvec4 Red		= Color(0xFF0000);
constexpr Fvec4 Green	= Color(0x00FF00);
constexpr Fvec4 Blue	= Color(0x0000FF);
constexpr Fvec4 Yellow	= Color(0xFFFF00);
constexpr Fvec4 Cyan	= Color(0x00FFFF);
constexpr Fvec4 Magenta = Color(0xFF00FF);
constexpr Fvec4 Gray	= Color(0x808080);

// Convert a HSV color into a RGB color (includes the alpha channel)
// @param hsva: the saturation and the value is in range of [0, 1], while the hue could be any real number.
constexpr Fvec4 HSVAToRGBA(Fvec4 hsva)
{
	MATHYW_ASSERT(hsva[1] >= 0.0f && hsva[1] <= 1.0f && hsva[2] >= 0.0f && hsva[2] <= 1.0f,
		"The parameter \"hsva\" in \"HSVAToRGB\" where the saturation and the value sould be in range of [0, 1]");
	for (; hsva[0] < 0.0f; hsva[0] += 360.0f); // negative hue ?
	for (; hsva[0] >= 360.0f; hsva[0] -= 360.0f); // hue >= 360 ?
	float c = hsva[2] * hsva[1];
	float h = hsva[0] / 60.0f;
	float hh = h;
	for (; hh >= 2.0f; hh -= 2.0f);
	float k = hh - 1.0f;
	float x = c * (1 - (k > 0 ? k : -k));
	Fvec3 tmp;
	if (h < 1) tmp = { c, x, 0 };
	else if (h < 2) tmp = { x, c, 0 };
	else if (h < 3) tmp = { 0, c, x };
	else if (h < 4) tmp = { 0, x, c };
	else if (h < 5) tmp = { x, 0, c };
	else tmp = { c, 0, x };
	float m = hsva[2] - c;
	tmp += Fvec3(m);
	return Fvec4(tmp[0], tmp[1], tmp[2], hsva[3]);
}

} // !Mathyw