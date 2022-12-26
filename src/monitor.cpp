#include <Mathyw/monitor.hpp>
#include <Mathyw/opengl.hpp>
#include <GLFW/glfw3.h>

namespace Mathyw {

static std::vector<Monitor> monitors;

Monitor::Monitor(void* monitor)
	: monitor(monitor) {}

Ivec2 Monitor::Size() const
{
	auto* vid = glfwGetVideoMode((GLFWmonitor*)monitor);
	return { vid->width, vid->height };
}

std::string Monitor::Name() const
{
	return glfwGetMonitorName((GLFWmonitor*)monitor);
}

std::vector<Monitor> const& GetMonitors()
{
	static bool is_constructed = false;
	if (is_constructed) return monitors;
	InitGL();
	int count;
	GLFWmonitor** ms = glfwGetMonitors(&count);
	monitors.reserve(count);
	for (int i = 0; i < count; i++)
		monitors.push_back(Monitor(ms[i]));
	is_constructed = true;
	return monitors;
}

Monitor const& PrimaryMonitor()
{
	return GetMonitors()[0];
}

} // !Mathyw