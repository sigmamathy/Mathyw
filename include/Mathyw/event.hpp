#pragma once

#include "./vector.hpp"
#include "./inputcode.hpp"

namespace Mathyw {

// A base event interface
struct Event
{
	// Returns the type of this event
	virtual std::uint8_t EventType() const = 0;
};

// Cast Event object to a derived event type, returns nullptr if failed.
template<class Ty> requires (std::is_same_v<decltype(Ty::StaticType), const std::uint8_t>)
Ty const* EventCast(Event const& e)
{
	if (e.EventType() == Ty::StaticType)
		return (Ty const*)&e;
	return nullptr;
}

// A lazy shortcut that defines the event type and override the function
#define MATHYW_EVENT_TYPE(val) static constexpr std::uint8_t StaticType = val;\
	inline std::uint8_t EventType() const override { return StaticType; }

// When users press or release a key
struct KeyEvent : public Event
{
	MATHYW_EVENT_TYPE(0);
	KeyEvent(KeyCode code, bool down) : keycode(code), down(down) {}
	KeyCode keycode; // the keycode defined in "InputCode.hpp"
	bool down; // true if key is pressed
};

// When users press or release a mouse button
struct MouseEvent : public Event
{
	MATHYW_EVENT_TYPE(1)
		MouseEvent(MouseButton button, bool down) : button(button), down(down) {}
	MouseButton button; // the mouse button defined in "InputCode.hpp"
	bool down; // true if button is pressed
};

// When users moves the mouse
struct MouseMovedEvent : public Event
{
	MATHYW_EVENT_TYPE(2)
		MouseMovedEvent(Ivec2 position) : position(position) {}
	Ivec2 position; // cursor position
};

// When users scroll the mouse (mouse wheel)
struct MouseScrolledEvent : public Event
{
	MATHYW_EVENT_TYPE(3)
		MouseScrolledEvent(Ivec2 position) : offset(offset) {}
	Ivec2 offset; // mouse offset
};

// When the window is focused or unfocused by the user
struct WindowFocusedEvent : public Event
{
	MATHYW_EVENT_TYPE(4)
		WindowFocusedEvent(bool focused) : focused(focused) {}
	bool focused; // true if focused
};

// When the window is closed by the user
struct WindowClosedEvent : public Event
{
	MATHYW_EVENT_TYPE(5)
};

// When the window is resized by the user
struct WindowResizedEvent : public Event
{
	MATHYW_EVENT_TYPE(6)
		WindowResizedEvent(Ivec2 size) : size(size) {}
	Ivec2 size; // current window size
};

// When the window is moved by the user
struct WindowMovedEvent : public Event
{
	MATHYW_EVENT_TYPE(7)
		WindowMovedEvent(Ivec2 position) : position(position) {}
	Ivec2 position; // current window position
};

#undef MATHYW_EVENT_TYPE

} // !Mathyw