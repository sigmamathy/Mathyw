#pragma once

#include "./monitor.hpp"
#include "./event.hpp"
#include "./color.hpp"

namespace Mathyw {

// A list of window hints that can be configured
enum : std::uint8_t
{
	WindowNone			= 0, // Nothing
	WindowResizable		= 1 << 0, // enable resizable
	WindowVisible		= 1 << 1, // enable visible
	WindowDecorated		= 1 << 2, // enable decoration
	WindowFocused		= 1 << 3, // focus the window
	WindowAutoIconify	= 1 << 4, // enable auto iconify
	WindowAlwaysOnTop	= 1 << 5, // enable always on top
	WindowMaximized		= 1 << 6, // maximize the window

	WindowDefault = WindowResizable	// default settings (0001 1111)
				  | WindowVisible
				  | WindowDecorated
				  | WindowFocused
				  | WindowAutoIconify
};

// Contains window data
struct WindowData final
{
	Ivec2 position, size;
	std::string title;
	bool active, has_vsync;
	std::function<void(Event const&)> callback;
	Ivec4 viewport;
};

// Manage and control window
class Window final
{
public:
	// Window initialization
	// @param width, height: the size of the window
	// @param title: the title of the window
	// @param hint: expects window hints defined above
	Window(int width, int height, std::string const& title, std::uint8_t hint = WindowDefault);

	// Fullscreen initialization
	// @param monitor: specify which monitor will the fullscreen window be
	// @param hint: expects window hints defined above
	Window(Monitor monitor, std::uint8_t hint = WindowDefault);

	// Destructor
	~Window();

	// No copy construct allowed (use move instead)
	Window(Window const&) = delete;

	// No reassignment operator
	Window& operator=(Window const&) = delete;

	// Move constructor (transfer ownership)
	Window(Window&&) noexcept;

	// Specify which window to use
	// @param window: could be nullptr if no window are used
	static void Bind(Window* window);

	// Returns the current window binded
	static Window* Current();

	// Equivalent to Bind(this)
	void Bind();

	// Poll window events and swap buffers
	void Update();

	// Close the window (would not call WindowClosedEvent)
	void Close();

	// Clear the window screen with color
	void Clear(Fvec4 color = Black);

	// Getters of window data
	inline Ivec2 Position() const { return data.position; }
	inline Ivec2 Size() const { return data.size; }
	inline std::string_view Title() const { return data.title; }
	inline Ivec4 Viewport() const { return data.viewport; }
	inline bool Vsync() const { return data.has_vsync; }

	// Returns false if window is closed, or Close() is called
	inline bool Active() const { return data.active; }

	// Setters of window data
	void Position(Ivec2 position);
	void Size(Ivec2 size);
	void Title(std::string const& title);
	void Viewport(Ivec4 viewport);
	void Vsync(bool enable);

	// Set window event callback
	void EventCallback(std::function<void(Window&, Event const&)> const& callback);

	// Set window event callback with additional parameter
	// @param params: list of pointer that will be directly passed into the callback
	template<class... Tys> requires (sizeof...(Tys) != 0)
	auto EventCallback(auto&& callback, Tys*... params)
	{
		EventCallback([=](Window& win, Event const& e) -> void {
			callback(win, e, params...);
		});
	}

	// Returns true if specific key is being pressed
	bool IsKeyPressed(KeyCode keycode) const;

	// Returns true if specific mouse button is being pressed
	bool IsMouseButtonPressed(MouseButton button) const;

	// Get the current mouse position, where the origin is on the top-left corner
	// and downard is the positive direction of y
	Ivec2 MousePosition() const;

private:
	void* window;
	WindowData data;
	bool destruct_this;
};

}