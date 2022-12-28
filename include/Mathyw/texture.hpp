#pragma once

#include "./vector.hpp"

namespace Mathyw {

// Stores texture for OpenGL shader bindings.
class Texture final
{
public:
	// Pass the image data into this texture
	// @param data: the array pointer of image
	// @param size: size of that image
	// @param channels: number of channels of image, expects 1 to 4
	Texture(std::uint8_t const* data, Ivec2 size, int channels);

	// Directly load the texture from file
	// @param channels: number of channels to be loaded (1 to 4), uses default if zero
	Texture(std::string const& path, int channels = 0);

	// No copy construct allowed (use move instead)
	Texture(Texture const&) = delete;

	// No reassignment operator
	Texture& operator=(Texture const&) = delete;

	// Move constructor (transfer ownership)
	Texture(Texture&&) noexcept;

	// Destructor
	~Texture();

	// Specify which texture to use
	// @param texture: could be nullptr if no texture are used
	// @param slot: indicates which texture slot to bind
	static void Bind(Texture* texture, int slot = 0);

	// Equivalent to Bind(this, slot)
	void Bind(int slot = 0);

	// Returns the size of the texture
	inline Ivec2 Size() const { return size; }

private:
	std::uint32_t textureid;
	Ivec2 size;
	bool destruct_this;
};

} // !Mathyw