#pragma once

#include "./vector.hpp"

namespace Mathyw {

// Allows user to manage OpenGL shader.
// Supports shader source loading (version 330 core), bindings and uniforms
class Shader
{
public:
	// Initialize with a shader source (vertex shader and fragment shader)
	Shader(std::string const& vertex, std::string const& fragment);

	// No copy construct allowed (use move instead)
	Shader(Shader const&) = delete;

	// No reassignment operator
	Shader& operator=(Shader const&) = delete;

	// Move constructor (transfer ownership)
	Shader(Shader&&) noexcept;

	// Destructor
	~Shader();

	// Return true if no error occurred
	inline operator bool() const { return good; }

	// The error message OpenGL reported, empty if no errors
	inline std::string_view ErrorMessage() const { return err_message; }

	// Specify which shader to use
	// @param shader: could be nullptr if no shader are used
	static void Bind(Shader* shader);

	// Returns the current shader binded
	static Shader* Current();

	// Equivalent to Bind(this)
	void Bind();

	// Set a uniform vector
	// @param name: the name of the uniform
	// @param vec: Type could be int, unsigned int or float, size could be 1 to 4
	template<class Ty, std::uint8_t Sz>
	void Uniform(std::string const& name, Vector<Ty, Sz> const& vec);

	// Set a uniform matrix
	// @param name: the name of the uniform
	// @param mat: Type must be float, rows and columns could be 1 to 4
	template<std::uint8_t R, std::uint8_t C>
	void Uniform(std::string const& name, Matrix<float, R, C> const& mat);

	// Set uniform, it follows the implementation of uniform vectors
	// @param name: the name of the uniform
	// @param args: sizeof...(args) must be 1 to 4
	template<ArithmeticType... Tys>
	void Uniform(std::string const& name, Tys... args)
	{
		Vector<std::common_type_t<Tys...>, sizeof...(Tys)> vec = { args... };
		Uniform(name, vec);
	}

protected:
	std::uint32_t shaderid;
	std::unordered_map<std::string, int> uniform_location_cache;
	bool good, destruct_this;
	std::string err_message;

	// Returns the location of the uniform
	int UniformLocation(std::string const& name);
};

// Read shaders from files (vertex shader and fragment shader)
Shader ReadShaderFile(std::string const& vertex, std::string const& fragment);

} // !Mathyw