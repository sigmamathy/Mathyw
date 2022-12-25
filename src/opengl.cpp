#include <Mathyw/opengl.hpp>
#include <GLFW/glfw3.h>

namespace Mathyw {

namespace {

	bool allow_gl_terminate = false;
	struct GLFWManager {
		GLFWManager();
		~GLFWManager();
	};
}

void InitGL()
{
	static GLFWManager instance;
	allow_gl_terminate = true;
}

void ExitGL()
{
	if (allow_gl_terminate)
		glfwTerminate();
	allow_gl_terminate = false;
}

GLFWManager::GLFWManager()
{
	glfwInit();
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