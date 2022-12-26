#pragma once

#include "./core.hpp"

namespace Mathyw {

// Vertex array draw primitives option
enum class Primitives : std::uint8_t
{
	Points,
	Lines,
	LineLoop,
	LineStrip,
	Triangles,
	TriangleStrip,
	TriangleFan
};

// Vertex array layout, calculates offsets and the stride. 
// Increments the attribute position as it goes
struct VertexLayout final
{
	// Initialize the layout
	// @param args: each indicates the count of the attribute
	template<class... Tys> requires (std::is_same_v<Tys, int> && ...)
	VertexLayout(Tys... args) { (Add(args), ...); }

	// Add a vertex attribute
	void Add(int count);

	// Vertex attributes
	struct Attribute final { int count, offset; };
	std::vector<Attribute> attributes;
	int stride = 0;
};

// OpenGL vertex array object, allows user to link multiple VBOs and IBO
class VertexArray
{
public:
	// Initialize the vertex array
	// @param count: how many indices there are gonna be
	// @param primitive: primitive option for drawing
	VertexArray(int count, Primitives primitive = Primitives::Triangles);

	// No copy construct allowed (use move instead)
	VertexArray(VertexArray const&) = delete;

	// No reassignment operator
	VertexArray& operator=(VertexArray const&) = delete;

	// Move constructor (transfer ownership)
	VertexArray(VertexArray&&) noexcept;

	// Destructor
	~VertexArray();

	// Specify which vao to use
	// @param vao: could be nullptr if no vao are used
	static void Bind(VertexArray* vao);

	// Returns the current vao binded
	static VertexArray* Current();

	// Equivalent to Bind(this)
	void Bind();

	// Link a vertex buffer object to this vao
	// @param data: the array pointer that points the data,
	//				the size of array must equal to the indices count in the constructor
	// @param layout: specify the layout for the specific buffer
	void LinkVBO(float const* data, VertexLayout layout);

	// Link a index buffer object to this vao
	// @param data: the array pointer that points to the indices data
	// @param count: the size of that array
	void LinkIBO(unsigned const* data, std::uint32_t count);

	// Link a vbo with a container object instead of pointers.
	// @param container: the specific conatiner that has operator[] and size() defined
	// @param layout: specify the layout for the specific buffer
	template<ContainerType Ty>
	void LinkVBO(Ty&& container, VertexLayout layout)
	{
		MATHYW_ASSERT(container.size() == layout.stride * count_indices,
			"Illegal size of container in LinkVBO");
		LinkVBO(&container[0], layout);
	}

	// Link a ibo with a container object instead of pointers.
	// @param container: the specific conatiner that has operator[] and size() defined
	template<ContainerType Ty>
	void LinkIBO(Ty&& container)
	{
		LinkIBO(&container[0], (std::uint32_t)container.size());
	}

	// Draw the vertex array object and pass vertex data into shader
	void Draw();

private:
	std::uint32_t vaoid, iboid;
	std::vector<std::uint32_t> vboids;
	std::uint32_t attribloc;
	int count_indices, elem_to_draw;
	Primitives primitives;
	bool destruct_this;
};

} // !Mathyw