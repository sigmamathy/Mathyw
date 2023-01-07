#include <Mathyw/all.hpp>
#include <filesystem>

void EventCallback(Mathyw::Window& window, Mathyw::Event const& e)
{
	if (Mathyw::EventCast<Mathyw::WindowClosedEvent>(e))
		window.Close();
}

int main(int argc, char** argv)
{
	Mathyw::Window window(1600, 900, "Hello World", Mathyw::WindowDefault ^ Mathyw::WindowResizable);
	window.Vsync(true);

	window.EventCallback(EventCallback);

	while (window.Active())
	{
		window.Clear(Mathyw::Black);

		window.Update();
	}
	
	return 0;
}