#include <Mathyw/window.hpp>

void EventCallback(Mathyw::Window& window, Mathyw::Event const& e)
{
	if (Mathyw::EventCast<Mathyw::WindowClosedEvent>(e))
		window.Close();

	if (auto* ke = Mathyw::EventCast<Mathyw::KeyEvent>(e))
	{
		if (ke->down)
			std::cout << ke->keycode << '\n';
	}
}

int main(int argc, char** argv)
{
	Mathyw::Window window(1600, 900, "Hello World");
	window.EventCallback(EventCallback);

	while (window.Active()) {
		window.Update();
	}
	
	return 0;
}