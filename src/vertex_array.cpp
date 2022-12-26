#include <Mathyw/vertex_array.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Mathyw {

void VertexLayout::Add(int count)
{
	attributes.emplace_back(count, stride);
	stride += count;
}

VertexArray::VertexArray(int count, Primitives primitive)
	: iboid(0), attribloc(0), count_indices(count), primitives(primitive), elem_to_draw(count), destruct_this(true)
{
	glGenVertexArrays(1, &vaoid);
	vboids.reserve(2);
}

VertexArray::VertexArray(VertexArray&& vao) noexcept
	: vaoid(vao.vaoid), iboid(vao.iboid), attribloc(vao.attribloc),
	count_indices(vao.count_indices), primitives(vao.primitives), elem_to_draw(vao.elem_to_draw), destruct_this(true)
{
	vao.destruct_this = false;
}

VertexArray::~VertexArray()
{
	if (!destruct_this) return;
	glDeleteVertexArrays(1, &vaoid);
	for (auto& buffer : vboids)
		glDeleteBuffers(1, &buffer);
	if (iboid)
		glDeleteBuffers(1, &iboid);
}

static VertexArray* binded_vao = nullptr;

void VertexArray::Bind(VertexArray* vao)
{
	if (vao == binded_vao) return;
	binded_vao = vao;
	if (vao) glBindVertexArray(vao->vaoid);
	else glBindVertexArray(0);
}

VertexArray* VertexArray::Current()
{
	return binded_vao;
}

void VertexArray::Bind()
{
	Bind(this);
}

void VertexArray::LinkVBO(float const* data, VertexLayout layout)
{
	MATHYW_ASSERT(data, "LinkVBO data cannot be null");
	auto& vboid = vboids.emplace_back();
	Bind();
	glGenBuffers(1, &vboid);
	glBindBuffer(GL_ARRAY_BUFFER, vboid);
	glBufferData(GL_ARRAY_BUFFER, layout.stride * count_indices * sizeof(float), data, GL_STATIC_DRAW);

	for (auto i = 0u; i < layout.attributes.size(); i++)
	{
		glEnableVertexAttribArray(attribloc + i);
		auto const& attrib = layout.attributes[i];
		glVertexAttribPointer(attribloc + i, attrib.count, GL_FLOAT, false,
			layout.stride * sizeof(float), (void*)(attrib.offset * sizeof(float)));
	}

	attribloc += (std::uint32_t)layout.attributes.size();
	Bind(nullptr);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexArray::LinkIBO(unsigned const* data, std::uint32_t count)
{
	MATHYW_ASSERT(data, "LinkIBO data cannot be null");
	Bind();
	if (iboid) glDeleteBuffers(1, &iboid);
	glGenBuffers(1, &iboid);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboid);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW);
	Bind(nullptr);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	elem_to_draw = count;
}

void VertexArray::Draw()
{
	Bind();
	if (iboid) glDrawElements((unsigned)primitives, elem_to_draw, GL_UNSIGNED_INT, 0);
	else glDrawArrays((unsigned)primitives, 0, count_indices);
}

} // !Mathyw