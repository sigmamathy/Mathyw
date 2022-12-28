#include <Mathyw/window.hpp>

int main(int argc, char** argv)
{
	// create a window object
	Mathyw::Window window(1600, 900, "Hello World");

	// while window is still active
	while (window.Active())
		// poll window events
		window.Update();

	return 0;
}