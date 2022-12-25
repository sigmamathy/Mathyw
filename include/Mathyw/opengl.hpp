#pragma once

#include "./core.hpp"

namespace Mathyw {

// Initialize GLFW library (glfw3.lib).
// This function would be automatically called if a GL context is required,
// though you can always call this if you want to implement your OpenGL code.
// Also be aware that you should never call glfwInit by your own which might cause undefined behaviour.
// Multiple calls of this function would have no effects.
void InitGL();

// Terminates GLFW library (glfw3.lib).
// This function would be automatically called when the program exited,
// though you can always call this if you want to exit dynamically.
// Also be aware that you should never call glfwTerminate by your own which might cause undefined behaviour.
// Multiple calls of this function would have no effects.
void ExitGL();

} // !Mathyw