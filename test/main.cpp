#include <Mathyw/all.hpp>

void EventCallback(Mathyw::Window& window, Mathyw::Event const& e)
{
	if (Mathyw::EventCast<Mathyw::WindowClosedEvent>(e))
		window.Close();

	if (auto* ke = Mathyw::EventCast<Mathyw::KeyEvent>(e))
	{
		if (ke->down)
			std::cout << ke->keycode << '\n';
	}
}

static const char* vshader = R"(

#version 330 core

layout (location = 0) in vec2 ipos;
uniform mat4 u_model, u_proj;

void main() {
	gl_Position = u_proj * u_model * vec4(ipos, 0.0f, 1.0f);
}

)";

static const char* fshader = R"(

#version 330 core

out vec4 FragColor;

void main() {
	FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);
}

)";

static constexpr std::array<float, 8> vertices = {
	-0.5f, 0.5f,
	0.5f, 0.5f,
	0.5f, -0.5f,
	-0.5f, -0.5f
};

static constexpr std::array<unsigned, 6> indices = {
	0, 1, 2,
	2, 3, 0
};

int main(int argc, char** argv)
{
	Mathyw::Window window(1600, 900, "Hello World", Mathyw::WindowDefault ^ Mathyw::WindowResizable);
	window.EventCallback(EventCallback);
	window.Vsync(true);

	Mathyw::VertexArray vao(4);
	vao.LinkVBO(vertices, Mathyw::VertexLayout(2));
	vao.LinkIBO(indices);
	Mathyw::Shader shader(vshader, fshader);
	shader.Uniform("u_model", Mathyw::Scale(Mathyw::Fvec3(50.0f)));
	shader.Uniform("u_proj", Mathyw::OrthogonalProjection(window.Size()));

	Mathyw::Clock clock;

	while (window.Active())
	{
		float elapsed = clock.Restart();

		window.Title("Hello World - FPS: " + std::to_string((int)(1.0f / elapsed)));
		window.Clear(Mathyw::Fvec4(0.07f, 0.13f, 0.17f, 1.0f));

		shader.Bind();
		vao.Draw();

		window.Update();
	}
	
	return 0;
}