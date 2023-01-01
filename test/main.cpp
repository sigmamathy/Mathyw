#include <Mathyw/all.hpp>
#include <filesystem>

void EventCallback(Mathyw::Window& window, Mathyw::Event const& e, Mathyw::DevMenu* menu)
{
	if (Mathyw::EventCast<Mathyw::WindowClosedEvent>(e))
		window.Close();
	menu->EventCall(e);
}

int main(int argc, char** argv)
{
	Mathyw::Window window(1600, 900, "Hello World", Mathyw::WindowDefault ^ Mathyw::WindowResizable);
	window.Vsync(true);

	std::string root = "../../../../test/";
	Mathyw::Font arial(root + "Arial.ttf", 30);
	auto projection = Mathyw::OrthogonalProjection(window.Size());

	Mathyw::DevMenu menu(window, "Favourite", arial);
	float x = 15.0f;
	menu.AddFloatScrollBarField("Hello", x, 10, 30);

	window.EventCallback(EventCallback, &menu);

	while (window.Active())
	{
		window.Clear(Mathyw::Black);

		menu.Render(projection);

		window.Update();
	}
	
	return 0;
}