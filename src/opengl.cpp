#include <Mathyw/opengl.hpp>
#include <GLFW/glfw3.h>

namespace Mathyw {

namespace {

	bool glfw_should_terminate = false;
	struct GLFWManager {
		GLFWManager();
		~GLFWManager();
	};
}

void InitGL()
{
	static GLFWManager instance;
	glfw_should_terminate = true;
}

void ExitGL()
{
	if (glfw_should_terminate)
		glfwTerminate();
	glfw_should_terminate = false;
}

GLFWManager::GLFWManager()
{
	MATHYW_VERIFY(glfwInit(), "GLFW initialization failed");
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(
		GLFW_OPENGL_PROFILE,
		GLFW_OPENGL_CORE_PROFILE);
}

GLFWManager::~GLFWManager()
{
	ExitGL();
}

}