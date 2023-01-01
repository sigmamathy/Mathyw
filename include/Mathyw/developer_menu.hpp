#pragma once

#include "./font.hpp"
#include "./window.hpp"

namespace Mathyw {

// Development menu field
struct DevMenuField {
	Font& font;
	std::string name;
	Fvec3 position, size;
	inline DevMenuField(Font& font, std::string_view name, Fvec3 pos, Fvec3 size)
		: font(font), name(name), position(pos), size(size) {}
	virtual void Render() = 0;
};

// A menu for development, having a nice UI to control variables
class DevMenu
{
public:
	// Menu initialization
	// @param window: where the menu panel is supposed to be in
	// @param title: title of this menu
	// @param font: the font used to render text
	DevMenu(Window& window, std::string_view title, Font& font);

	// No copy construct allowed (use move instead)
	DevMenu(DevMenu const&) = delete;

	// No reassignment operator
	DevMenu& operator=(DevMenu const&) = delete;

	// Getters of menu information
	inline Fvec3 Position() const { return position; }
	inline Fvec3 Size() const { return size; }

	// Setters of menu information
	void Position(Fvec3 position);
	void Size(Fvec3 size);

	// Render the entire menu at once
	// no extra resources required as they're already provided internally
	// @param projection: expects a orthoganol window projection
	void Render(Fmat4 const& projection);

	// Pass window event to this menu
	void EventCall(Event const& e);

	// Add a float scroll bar field to menu
	// @param ref: the output value reference, will automatically clamp to [min, max]
	void AddFloatScrollBarField(std::string_view name, float& ref, float min, float max);

private:
	Window& window;
	Fvec3 position, size;
	Text title;
	Font& font;
	std::vector<std::unique_ptr<DevMenuField>> fields;
	// temporary storage
	Fvec2 prev_mpos;
	bool title_fclick, sidebar_fclick;
};

} // !Mathyw