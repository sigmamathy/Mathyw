#pragma once

#include "./matrix.hpp"

namespace Mathyw {

// A class template for various type of vectors, inherit from Matrix
// std::uint8_t is used for vector size since dimensions larger than 255 are not intented to be used.
template<ArithmeticType Ty, std::uint8_t Sz>
class Vector : public Matrix<Ty, Sz, 1>
{
public:
	// Inherit super functions
	static constexpr std::uint8_t Size = Sz;
	using Matrix<Ty, Sz, 1>::Matrix;
	using Matrix<Ty, Sz, 1>::operator[];

	// Value uninitialized constructor
	constexpr Vector() = default;

	// Fills all component with certain value
	// @param value: the initialized value
	constexpr Vector(Ty value)
	{
		for (std::uint8_t i = 0u; i < Size; i++)
			Get(i) = value;
	}

	// Copy constructor of matrices
	template<class Ty2>
	constexpr Vector(Matrix<Ty2, Sz, 1> const& mat)
	{
		for (std::uint8_t i = 0u; i < Size; i++)
			Get(i) = (Ty)mat.Get(i, 0);
	}

	// Directly access element via indexing (overrided matrix method)
	constexpr Ty& Get(std::uint8_t index)
	{
		MATHYW_ASSERT(index < Size, "Vector index out of bounds in Get");
		return (*this)[index];
	}

	// Directly access element via indexing (overrided matrix method)
	constexpr Ty const& Get(std::uint8_t index) const
	{
		MATHYW_ASSERT(index < Size, "Vector index out of bounds in Get");
		return (*this)[index];
	}

	// Access multiple elements at the same time and extract it as a vector
	// @param indices: a list of index that indicates elements position inside the vector
	template<class... Tys>
		requires (sizeof...(Tys) > 1
		&& (std::is_convertible_v<Tys, std::uint8_t> && ...))
	constexpr auto Get(Tys... indices) const
	{
		Vector<Ty, sizeof...(Tys)> res;
		std::array<std::uint8_t, sizeof...(Tys)> const inds = { indices... };
		for (std::uint8_t i = 0u; i < sizeof...(Tys); i++)
			res[i] = Get(inds[i]);
		return res;
	}

	// Overloads the += operator
	template<class Ty2>
	constexpr auto operator+=(Vector<Ty2, Sz> const& vec)
	{
		for (std::uint8_t i = 0u; i < Size; i++)
			Get(i) += Ty(vec[i]);
		return *this;
	}

	// Overloads the -= operator
	template<class Ty2>
	constexpr auto operator-=(Vector<Ty2, Sz> const& vec)
	{
		for (std::uint8_t i = 0u; i < Size; i++)
			Get(i) -= Ty(vec[i]);
		return *this;
	}

	// Overloads the *= operator
	template<ArithmeticType STy>
	constexpr auto operator*=(STy scale)
	{
		for (std::uint8_t i = 0u; i < Size; i++)
			Get(i) *= Ty(scale);
		return *this;
	}

	// Calculate the norm of vector (aka magnitude)
	constexpr std::common_type_t<Ty, float> Norm() const
	{
		float sum = 0.0f;
		for (std::uint8_t i = 0u; i < Size; i++)
			sum += Ty(Get(i) * Get(i));
		return Power(sum, 0.5f);
	}
};

// The default method of printing vector (for debug purpose)
// @param os: expects std::cout
template<class Ty, std::uint8_t Sz>
std::ostream& operator<<(std::ostream& os, Vector<Ty, Sz> const& vec)
{
	os << '[' << vec[0];
	for (std::uint8_t i = 1u; i < Sz; i++)
		os << ", " << vec[i];
	return os << ']';
}

// Add two vectors, operator overloaded
template<class Ty1, class Ty2, std::uint8_t Sz>
constexpr auto operator+(Vector<Ty1, Sz> const& vec1, Vector<Ty2, Sz> const& vec2)
{
	return Vector<decltype(vec1[0] + vec2[0]), Sz>(
		(Matrix<Ty1, Sz, 1>) vec1 + vec2
		);
}

// Subtract two vectors, operator overloaded
template<class Ty1, class Ty2, std::uint8_t Sz>
constexpr auto operator-(Vector<Ty1, Sz> const& vec1, Vector<Ty2, Sz> const& vec2)
{
	return Vector<decltype(vec1[0] - vec2[0]), Sz>(
		(Matrix<Ty1, Sz, 1>) vec1 - vec2
		);
}

// Negate vector, operator overloaded
template<class Ty, std::uint8_t Sz>
constexpr auto operator-(Vector<Ty, Sz> const& vec)
{
	return decltype(vec) (-(Matrix<Ty, Sz, 1>) vec);
}

// Scaler multiplication of vector, operator overloaded
template<class Ty, ArithmeticType STy, std::uint8_t Sz>
constexpr auto operator*(Vector<Ty, Sz> const& vec, STy scale)
{
	return (Matrix<Ty, Sz, 1>) vec * scale;
}

// Scaler multiplication of vector, operator overloaded
template<class Ty, ArithmeticType STy, std::uint8_t Sz>
constexpr auto operator*(STy scale, Vector<Ty, Sz> const& vec)
{
	return scale * (Matrix<Ty, Sz, 1>) vec;
}

// Scaler multiplication of vector with division, operator overloaded
template<class Ty, ArithmeticType STy, std::uint8_t Sz>
constexpr auto operator/(Vector<Ty, Sz> const& vec, STy scale)
{
	return (Matrix<Ty, Sz, 1>) vec / scale;
}

// Dot product of two vectors
template<class Ty1, class Ty2, std::uint8_t Sz>
constexpr auto Dot(Vector<Ty1, Sz> const& vec1, Vector<Ty2, Sz> const& vec2)
{
	decltype(vec1[0] * vec2[0]) res = 0;
	for (std::uint8_t i = 0u; i < Sz; i++)
		res += vec1[i] * vec2[i];
	return res;
}

// Cross product of two vectors (2 dimensional)
template<class Ty1, class Ty2>
constexpr auto Cross(Vector<Ty1, 2> const& vec1, Vector<Ty2, 2> const& vec2)
{
	return vec1[0] * vec2[1] - vec1[1] * vec2[0];
}

// Cross product of two vectors (3 dimensional)
template<class Ty1, class Ty2>
constexpr auto Cross(Vector<Ty1, 3> const& vec1, Vector<Ty2, 3> const& vec2)
{
	return Vector<decltype(vec1[0] * vec2[0]), 3>(
		vec1[1] * vec2[2] - vec1[2] * vec2[1],
		vec1[2] * vec2[0] - vec1[0] * vec2[2],
		vec1[0] * vec2[1] - vec1[1] * vec2[0]
		);
}

// Hadamard product of vectors
template<class Ty1, class Ty2, std::uint8_t Sz>
constexpr auto Hadamard(Vector<Ty1, Sz> const& vec1, Vector<Ty2, Sz> const& vec2)
{
	return (Vector<decltype(vec1[0] * vec2[0]), Sz>)
		NtHadamard((Matrix<Ty1, Sz, 1>) vec1, vec2);
}

// Normalize a vector
template<class Ty, std::uint8_t Sz>
constexpr auto Normalize(Vector<Ty, Sz> const& vec)
{
	using common_type = std::common_type_t<Ty, float>;
	Ty sum = 0;
	for (std::uint8_t i = 0u; i < Sz; i++)
		sum += vec[i] * vec[i];
	auto invsqrt = (common_type) 1.0f / Power(float(sum), 0.5f);
	Vector<common_type, Sz> res;
	for (std::uint8_t i = 0u; i < Sz; i++)
		res[i] = vec[i] * invsqrt;
	return res;
}

// Integer type vectors
using Ivec2 = Vector<int, 2>;
using Ivec3 = Vector<int, 3>;
using Ivec4 = Vector<int, 4>;

// Unsigned integer type vectors
using Uvec2 = Vector<unsigned int, 2>;
using Uvec3 = Vector<unsigned int, 3>;
using Uvec4 = Vector<unsigned int, 4>;

// Float type vectors
using Fvec2 = Vector<float, 2>;
using Fvec3 = Vector<float, 3>;
using Fvec4 = Vector<float, 4>;

} // !Mathyw