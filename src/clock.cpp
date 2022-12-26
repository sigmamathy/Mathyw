#include <Mathyw/clock.hpp>
#include <Mathyw/opengl.hpp>
#include <GLFW/glfw3.h>

namespace Mathyw {

Clock::Clock()
{
	InitGL();
	timepoint = static_cast<float>(glfwGetTime());
}

float Clock::Elapsed() const
{
	auto now = static_cast<float>(glfwGetTime());
	return now - timepoint;
}

float Clock::Restart()
{
	float x = Elapsed();
	timepoint = static_cast<float>(glfwGetTime());
	return x;
}

}