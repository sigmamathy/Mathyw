#pragma once

#if !defined(__cplusplus)
#error cpp is required
#endif

#if defined (_MSC_VER) // msvc compiler
#define MATHYW_CXX_VERSION _MSVC_LANG
#elif defined(__GNUC__) || defined(__clang__) // gcc or clang
#define MATHYW_CXX_VERSION __cplusplus
#else // otherwise
#error unknown compiler detected, only MSVC, GCC and CLANG are supported
#endif

#if MATHYW_CXX_VERSION < 202002L // version lower than C++20
#error C++20 is the minimum version requried, try compile with /std:c++20 or -std=c++20
#endif

// Mathyw supported platforms
#define MATHYW_WINDOWS	0x0
#define MATHYW_MACOS	0x1
#define MATHYW_LINUX	0x2

#if defined(_WIN32) || defined(_WIN64) // windows
#define MATHYW_PLATFORM MATHYW_WINDOWS
#elif defined(__APPLE__) // macos
#define MATHYW_PLATFORM MATHYW_MACOS
#elif defined(__linux__) // linux
#define MATHYW_PLATFORM MATHYW_LINUX
#else // otherwise
#error unknown platform detected, only Windows, MacOS and Linux are supported
#endif

#if MATHYW_PLATFORM == MATHYW_WINDOWS
#ifdef _DEBUG // MSVC Debug
#define MATHYW_DEBUG 1
#endif
#elif MATHYW_PLATFORM == MATHYW_MACOS || MATHYW_PLATFORM == MATHYW_LINUX
#ifdef __OPTIMIZE__ // GCC or clang Debug
#define MATHYW_DEBUG 1
#endif
#endif

#ifdef MATHYW_DEBUG // Minimal check on debug
#define MATHYW_ASSERT(x, ...) if (!(x)) throw std::exception(__VA_ARGS__)
#define MATHYW_VERIFY(x, ...) MATHYW_ASSERT(x, __VA_ARGS__)
#else // No check on release
#define MATHYW_ASSERT(x, ...)
#define MATHYW_VERIFY(x, ...) x
#endif

// Mathyw C++ standard libraries dependencies
#include <string>
#include <array>
#include <vector>
#include <unordered_map>
#include <functional>
#include <iostream>
#include <type_traits>
#include <cmath>

namespace Mathyw {

// Success if such type is an arithemtic type (primary data types)
template<class Ty>
concept ArithmeticType = std::is_arithmetic_v<Ty>;

} // !Mathyw