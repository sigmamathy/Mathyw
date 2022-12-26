#pragma once

#include "./vector.hpp"

namespace Mathyw {

// Provide information of the monitor
class Monitor final
{
public:
	// Returns the size of the monitor
	Ivec2 Size() const;

	// Returns the name of the monitor
	std::string Name() const;

private:
	// Monitor pointer
	void* monitor;

	// OpenGL private constructor
	Monitor(void* monitor);

	// Friend classes and functions
	friend class Window;
	friend std::vector<Monitor> const& GetMonitors();
};

// Returns all available monitors in the computer
std::vector<Monitor> const& GetMonitors();

// Returns the primary monitors in the computer
// Equivalent to Monitors()[0]
Monitor const& PrimaryMonitor();

} // !Mathyw