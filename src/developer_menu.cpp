#include <Mathyw/developer_menu.hpp>
#include <Mathyw/shader.hpp>
#include <Mathyw/vertex_array.hpp>
#include <Mathyw/color.hpp>
#include <Mathyw/transformation.hpp>

namespace Mathyw {

namespace resources {

static char const* vshader = R"(

#version 330 core
layout(location = 0) in vec2 ipos;
layout(location = 1) in vec2 itexcoord;
uniform mat4 u_model, u_projection;
out vec2 vtexcoord;
void main() {
	gl_Position = u_projection * u_model * vec4(ipos, 0.0f, 1.0f);
	vtexcoord = itexcoord;
}

)";

static char const* fshader = R"(

#version 330 core
out vec4 FragColor;
in vec2 vtexcoord;
uniform vec4 u_color;
uniform bool u_hastexture;
uniform sampler2D u_texture;
uniform vec2 u_window_size, u_panel_pos, u_panel_size;
void main() {
	vec2 fpos = gl_FragCoord.xy - u_window_size / 2.0f;
	vec2 hsz = u_panel_size / 2.0f;
	if (fpos.x >= u_panel_pos.x - hsz.x && fpos.x <= u_panel_pos.x + hsz.x
		&& fpos.y >= u_panel_pos.y - hsz.y && fpos.y <= u_panel_pos.y + hsz.y)
		FragColor = u_color * (u_hastexture ? texture(u_texture, vtexcoord) : vec4(1.0f));
	else
		FragColor = vec4(0.0f);
}

)";

constexpr static std::array<float, 24> square_vertices = {
	-0.5f, -0.5f, 0.0f, 0.0f,
	-0.5f, 0.5f, 0.0f, 1.0f,
	0.5f, -0.5f, 1.0f, 0.0f,
	-0.5f, 0.5f, 0.0f, 1.0f,
	0.5f, -0.5f, 1.0f, 0.0f,
	0.5f, 0.5f, 1.0f, 1.0f,
};

constexpr static std::array<float, 12> triangle_vertices = {
	0.0f, 0.5f, 0.5f, 1.0f,
	-0.433f, -0.25f, 0.067f, 0.25f,
	0.433f, -0.25f, 0.933f, 0.25f
};

} // !resources

// Main shader resource
static Shader& GetDevShaderResources() {
	static Shader instance(resources::vshader, resources::fshader);
	if (!instance)
		std::cout << instance.ErrorMessage() << '\n';
	return instance;
}

// Main Square vao
static VertexArray& GetDevSquareVAO()
{
	static struct Instance {
		VertexArray vao;
		Instance() : vao(6) {
			// run only once
			vao.LinkVBO(resources::square_vertices, VertexLayout(2, 2));
		}
	} instance;
	return instance.vao;
}

// Main triangle vao
static VertexArray& GetDevTriangleVAO()
{
	static struct Instance {
		VertexArray vao;
		Instance() : vao(3) {
			// run only once
			vao.LinkVBO(resources::triangle_vertices, VertexLayout(2, 2));
		}
	} instance;
	return instance.vao;
}

DevMenu::DevMenu(Window& window, std::string_view title, Font& font)
	: window(window), position(0.0f), size(400.0f, 300.0f, 1.0f), title(font, title), font(font)
{
	fields.reserve(5);
	prev_mpos = window.MousePosition();
	title_fclick = sidebar_fclick = false;
}

void DevMenu::Render(Fmat4 const& projection)
{
	Shader& shader = GetDevShaderResources();
	shader.Uniform("u_projection", projection);
	shader.Uniform("u_window_size", (Mathyw::Fvec2)window.Size());
	shader.Uniform("u_panel_pos", position.Get(0, 1));
	shader.Uniform("u_panel_size", size.Get(0, 1));
	shader.Uniform("u_hastexture", 0);
	shader.Uniform("u_texture", 0);

	// render main menu panel
	shader.Uniform("u_model", Translate(position) * Scale(size));
	shader.Uniform("u_color", Color(0x375473));
	GetDevSquareVAO().Draw();

	// render title bar
	shader.Uniform("u_model", Translate(position + Mathyw::Fvec3(0.0f, size[1] / 2.0f - 15.0f, 0.0f))
		* Scale(Mathyw::Fvec3(size[0], 30.0f, 1.0f)));
	shader.Uniform("u_color", Color(0x2a3861));
	GetDevSquareVAO().Draw();

	// render expand button
	shader.Uniform("u_model",
		Translate(position + Mathyw::Fvec3(-size[0] / 2.0f + 18.0f, size[1] / 2.0f - 16.0f, 0.0f))
		* Scale(20.0f));
	shader.Uniform("u_color", Mathyw::White);
	GetDevTriangleVAO().Draw();

	// render title
	shader.Uniform("u_hastexture", 1);
	for (auto title_model = 
		Translate(position + Mathyw::Fvec3(
			-size[0] / 2.0f + 32.0f, size[1] / 2.0f - 22.5f, 0.0f))
		* Scale(15.0f / title.Size()[1]);
		auto& ch : title.Characters())
	{
		shader.Uniform("u_model", title_model * ch.model);
		ch.texture.Bind(0);
		GetDevSquareVAO().Draw();
	}

	// render side bar
	shader.Uniform("u_hastexture", 0);
	shader.Uniform("u_model",
		Translate(position + Mathyw::Fvec3(size[0] / 2.0f - 5.0f, -15.0f, 0.0f))
		* Scale(Mathyw::Fvec3(10.0f, size[1] - 30.0f, 1.0f)));
	shader.Uniform("u_color", Mathyw::Color(0x194a61));
	GetDevSquareVAO().Draw();

	// render fields
	for (auto& field : fields)
		field->Render();
}

void DevMenu::Position(Fvec3 position)
{
	auto hs = size / 2.0f;
	auto whs = window.Size() / 2.0f;
	if (position[0] < hs[0] - whs[0]) position[0] = hs[0] - whs[0];
	if (position[0] > whs[0] - hs[0]) position[0] = whs[0] - hs[0];
	if (position[1] < hs[1] - whs[1]) position[1] = hs[1] - whs[1];
	if (position[1] > whs[1] - hs[1]) position[1] = whs[1] - hs[1];
	this->position = position;
	// update fields as well
	constexpr float height = 50.0f;
	for (int i = 0; i < fields.size(); i++) {
		fields[i]->position = position
			+ Mathyw::Fvec3(-5.0f, size[1] / 2.0f - 30.0f - height * (i + 0.5f), 0.0f);
	}
}

void DevMenu::Size(Fvec3 size)
{
	if (size[0] < 150.0f) size[0] = 150.0f;
	if (size[0] > window.Size()[0]) size[0] = window.Size()[0];
	if (size[1] < 50.0f) size[1] = 50.0f;
	if (size[1] > window.Size()[1]) size[1] = window.Size()[1];
	this->size = size;
	// update fields as well
	for (int i = 0; i < fields.size(); i++) {
		fields[i]->size[0] = size[0] - 10.0f;
	}
}

static bool MouseIn(Fvec2 mpos, Fvec2 boxp, Fvec2 boxs)
{
	Fvec2 hs = boxs / 2.0f;
	return mpos[0] >= boxp[0] - hs[0]
		&& mpos[0] <= boxp[0] + hs[0]
		&& mpos[1] >= boxp[1] - hs[1]
		&& mpos[1] <= boxp[1] + hs[1];
}

void DevMenu::EventCall(Event const& e)
{
	if (auto* me = EventCast<MouseEvent>(e))
	{
		if (me->down && me->button == MouseButton1) {
			auto mpos = window.MousePosition() - window.Size() / 2.0f;
			mpos[1] *= -1.0f;
			if (window.IsMouseButtonPressed(MouseButton1))
			{
				title_fclick = MouseIn(mpos, position.Get(0, 1) + Mathyw::Fvec2(0.0f, size[1] / 2.0f - 15.0f),
					Mathyw::Fvec2(size[0], 30.0f));
				sidebar_fclick = MouseIn(mpos, position.Get(0, 1) + Mathyw::Fvec2(size[0] / 2.0f - 5.0f, -15.0f),
					Mathyw::Fvec2(10.0f, size[1] - 30.0f));
			}
		}
		if (!me->down && me->button == MouseButton1) {
			title_fclick = sidebar_fclick = false;
		}
	}
	if (auto* mme = EventCast<MouseMovedEvent>(e))
	{
		auto mpos = window.MousePosition() - window.Size() / 2.0f;
		mpos[1] *= -1.0f;
		if (title_fclick)
		{
			auto ds = mpos - prev_mpos;
			Position(Position() + Mathyw::Fvec3(ds[0], ds[1], 0.0f));
		}
		if (sidebar_fclick)
		{
			auto ds = mpos - prev_mpos;
			Size(Size() + Mathyw::Fvec3(ds[0], -ds[1], 0.0f));
			Position(Position() + Mathyw::Fvec3(
				Size()[0] > 150.0f ? ds[0] / 2.0f : 0.0f,
				Size()[1] > 50.0f ? ds[1] / 2.0f : 0.0f, 0.0f));
		}
		prev_mpos = mpos;
	}
}

struct DevMenuFloatScrollBarField : public DevMenuField
{
	float min, max;
	float& ref;
	DevMenuFloatScrollBarField(
		Font& font, Fvec3 position, Fvec3 size,
		std::string_view name, float& ref, float min, float max)
		: DevMenuField(font, name, position, size), ref(ref) , min(min), max(max) {}
	void Render() override
	{
		Shader& shader = GetDevShaderResources();
		shader.Uniform("u_model", Translate(position) * Scale(size));
		shader.Uniform("u_color", Color(0x43637d));
		GetDevSquareVAO().Draw();
	}
};

void DevMenu::AddFloatScrollBarField(std::string_view name, float& ref, float min, float max)
{
	constexpr float height = 50.0f;
	auto pos = position + Mathyw::Fvec3(-5.0f, size[1] / 2.0f - 30.0f - height * (fields.size() + 0.5f), 0.0f);
	auto sz = Mathyw::Fvec3(size[0] - 10.0f, height, 0.0f);
	fields.emplace_back(std::make_unique<DevMenuFloatScrollBarField>(
		font, pos, sz, name, ref, min, max
	));
}

}