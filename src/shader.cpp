#include <Mathyw/shader.hpp>
#include <fstream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Mathyw {

static unsigned create_shader(unsigned type, std::string const& src, bool& good, std::string& err_message)
{
	auto shader = glCreateShader(type);
	const char* cstr = src.c_str();
	glShaderSource(shader, 1, &cstr, nullptr);
	glCompileShader(shader);
	int status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (!status)
	{
		std::string buffer;
		buffer.resize(512);
		glGetShaderInfoLog(shader, 512, NULL, &buffer[0]);
		good = false;
		err_message += "Following error found in "
			+ std::string(type == GL_VERTEX_SHADER ? "vertex shader:\n" : "fragment shader:\n") + buffer;
	}
	return shader;
}

Shader::Shader(std::string const& vertex, std::string const& fragment)
{
	shaderid = glCreateProgram();
	destruct_this = true;
	good = true;
	unsigned int vshader = create_shader(GL_VERTEX_SHADER, vertex, good, err_message);
	unsigned int fshader = create_shader(GL_FRAGMENT_SHADER, fragment, good, err_message);
	glAttachShader(shaderid, vshader);
	glAttachShader(shaderid, fshader);
	glLinkProgram(shaderid);
	glDeleteShader(vshader);
	glDeleteShader(fshader);
}

Shader::Shader(Shader&& shader) noexcept
	: good(shader.good), shaderid(shader.shaderid), uniform_location_cache(shader.uniform_location_cache), destruct_this(true)
{
	shader.destruct_this = false;
}

static void ReadFile(std::string const& file, std::string& output)
{
	std::ifstream ifs(file);
	MATHYW_ASSERT(ifs.is_open(), "Cannot open file \"" + file + "\"");
	std::string line;
	while (getline(ifs, line))
		output += line + '\n';
}

Shader ReadShaderFile(std::string const& vertex, std::string const& fragment)
{
	std::string vsrc, fsrc;
	ReadFile(vertex, vsrc), ReadFile(fragment, fsrc);
	return Shader(vsrc, fsrc);
}

Shader::~Shader()
{
	if (destruct_this)
		glDeleteProgram(shaderid);
}

static Shader* binded_shader = nullptr;

void Shader::Bind(Shader* shader)
{
	if (shader == binded_shader) return;
	binded_shader = shader;
	if (shader) glUseProgram(shader->shaderid);
	else glUseProgram(0);
}

Shader* Shader::Current()
{
	return binded_shader;
}

void Shader::Bind()
{
	Bind(this);
}

#define MATHYW_UNIFORM_VECTOR_FUNCTION(ty, sz, fn) template<> void Shader::Uniform(std::string const& name, Vector<ty, sz> const& vec)\
	{ Bind(); int location = UniformLocation(name); fn(location, 1, &vec[0]); }

MATHYW_UNIFORM_VECTOR_FUNCTION(int, 1, glUniform1iv)
MATHYW_UNIFORM_VECTOR_FUNCTION(int, 2, glUniform2iv)
MATHYW_UNIFORM_VECTOR_FUNCTION(int, 3, glUniform3iv)
MATHYW_UNIFORM_VECTOR_FUNCTION(int, 4, glUniform4iv)

MATHYW_UNIFORM_VECTOR_FUNCTION(unsigned, 1, glUniform1uiv)
MATHYW_UNIFORM_VECTOR_FUNCTION(unsigned, 2, glUniform2uiv)
MATHYW_UNIFORM_VECTOR_FUNCTION(unsigned, 3, glUniform3uiv)
MATHYW_UNIFORM_VECTOR_FUNCTION(unsigned, 4, glUniform4uiv)

MATHYW_UNIFORM_VECTOR_FUNCTION(float, 1, glUniform1fv)
MATHYW_UNIFORM_VECTOR_FUNCTION(float, 2, glUniform2fv)
MATHYW_UNIFORM_VECTOR_FUNCTION(float, 3, glUniform3fv)
MATHYW_UNIFORM_VECTOR_FUNCTION(float, 4, glUniform4fv)

#define MATHYW_UNIFORM_MATRIX_FUNCTION(r, c, fn) template<> void Shader::Uniform(std::string const& name, Matrix<float, r, c> const& mat)\
	{ Bind(); int location = UniformLocation(name); fn(location, 1, true, &mat[0]); }

MATHYW_UNIFORM_MATRIX_FUNCTION(2, 2, glUniformMatrix2fv)
MATHYW_UNIFORM_MATRIX_FUNCTION(2, 3, glUniformMatrix2x3fv)
MATHYW_UNIFORM_MATRIX_FUNCTION(2, 4, glUniformMatrix2x4fv)

MATHYW_UNIFORM_MATRIX_FUNCTION(3, 2, glUniformMatrix3x2fv)
MATHYW_UNIFORM_MATRIX_FUNCTION(3, 3, glUniformMatrix3fv)
MATHYW_UNIFORM_MATRIX_FUNCTION(3, 4, glUniformMatrix3x4fv)

MATHYW_UNIFORM_MATRIX_FUNCTION(4, 2, glUniformMatrix4x2fv)
MATHYW_UNIFORM_MATRIX_FUNCTION(4, 3, glUniformMatrix4x3fv)
MATHYW_UNIFORM_MATRIX_FUNCTION(4, 4, glUniformMatrix4fv)

int Shader::UniformLocation(std::string const& name)
{
	if (uniform_location_cache.count(name))
		return uniform_location_cache[name];
	int location = glGetUniformLocation(shaderid, name.c_str());
	uniform_location_cache[name] = location;
	return location;
}

#undef MATHYW_UNIFORM_VECTOR_FUNCTION
#undef MATHYW_UNIFORM_MATRIX_FUNCTION

}