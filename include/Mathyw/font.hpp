#pragma once

#include "./texture.hpp"

namespace Mathyw {

class Font;

// Stores the glyph data of each character
struct Glyph final
{
	Texture texture;
	Ivec2 size, bearing;
	unsigned advance;
};

// Handles text object generated by font
class Text final
{
public:
	// Initialize an text object, font is required
	Text(Font& font, std::string_view string);

	// Set string to text
	void String(std::string_view string);

	// An alternative function to assign string
	Text& operator=(std::string_view string);

	// Returns the string
	inline std::string_view String() const { return string; }

	// A specific character of the text generated.
	// Contains the model matrix and the texture reference.
	struct Character final
	{
		Texture& texture;
		Fmat4 model;
	};

	// Returns a list of characters
	inline std::vector<Character>& Characters() { return characters; }

	// Returns a list of characters
	inline std::vector<Character> const& Characters() const { return characters; }

	// Returns the size of the text
	inline Fvec2 Size() const { return size; }

private:
	Font& font;
	std::string string;
	std::vector<Character> characters;
	Fvec2 size;
};

// Load font (truetype) and manage them.
class Font final
{
public:
	// Load a truetype font through a filepath
	// @param pixel_size: the size of the pixel to be loaded
	Font(std::string const& path, int pixel_size);

	// Get a character glyph from the font
	Glyph& operator[](unsigned char c);

	// Get a character glyph from the font
	Glyph const& operator[](unsigned char c) const;

	// Generates a text object with calculated model matrices
	Text operator[](std::string const& text);

private:
	std::unordered_map<unsigned char, Glyph> glyphs;
	friend class Text;
};

} // !Mathyw