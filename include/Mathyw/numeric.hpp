#pragma once

#include "./core.hpp"

namespace Mathyw {

// List of all mathematical constant
namespace constant {

	constexpr float Pi					= 3.14159265358979323846f;
	constexpr float E					= 2.71828182845904523536f;
	constexpr float GoldenRatio			= 1.61803398874989484820f;
	constexpr float SilverRatio			= 2.41421356237309504880f;
	constexpr float SuperGoldenRatio	= 1.46557123187676802665f;
} // !Mathyw::constant

// Power function, use for loop implementation if ExpTy is an integral type,
// otherwise std::pow from <cmath> will be used
template<ArithmeticType BaseTy, ArithmeticType ExpTy>
constexpr std::common_type_t<BaseTy, ExpTy> Power(BaseTy base, ExpTy exp)
{
	if (!std::is_integral_v<ExpTy>)
		return std::pow(base, exp);

	BaseTy result = 1;
	for (int i = 0; i < exp; i++)
		result *= base;
	return std::is_signed_v<ExpTy> && exp >= 0 ? base : 1 / base;
}

// General logarithm inherit from <cmath>
template<ArithmeticType BaseTy, ArithmeticType ValTy>
constexpr std::common_type_t<ValTy, float> Logarithm(BaseTy base, ValTy val) {
	return std::log(val) / std::log(base);
}

// Turns radian to degree
template<ArithmeticType Ty>
constexpr std::common_type_t<Ty, float> Degree(Ty val) {
	return val * 180 / constant::Pi;
}

// Turns degree to radian
template<ArithmeticType Ty>
constexpr std::common_type_t<Ty, float> Radian(Ty val) {
	return val * constant::Pi / 180;
}

// Returns the absolute value of val
template<ArithmeticType Ty>
constexpr Ty Absolute(Ty val) {
	return val > 0 ? val : -val;
}

// Returns 1 if val > 0, returns 0 if val == 0, otherwise returns -1
template<ArithmeticType Ty>
constexpr int Signum(Ty val) {
	if (val == 0) return 0;
	return val > 0 ? 1 : -1;
}

}