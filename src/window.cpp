#include <Mathyw/window.hpp>
#include <Mathyw/opengl.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Mathyw {

static void CreateWindowEventCallback(GLFWwindow* window)
{
	glfwSetKeyCallback(window,
	[](GLFWwindow* window, int key, int scancode, int act, int modes) {
		auto& data = *(WindowData*)glfwGetWindowUserPointer(window);
		if (act != GLFW_REPEAT)
			data.callback(KeyEvent(KeyCode(key), act == GLFW_PRESS));
	});

	glfwSetMouseButtonCallback(window,
	[](GLFWwindow* window, int button, int act, int modes) {
		auto& data = *(WindowData*)glfwGetWindowUserPointer(window);
		if (act != GLFW_REPEAT)
			data.callback(MouseEvent(MouseButton(button), act == GLFW_PRESS));
	});

	glfwSetCursorPosCallback(window,
	[](GLFWwindow* window, double x, double y) {
		auto& data = *(WindowData*)glfwGetWindowUserPointer(window);
		Ivec2 res(x, y);
		data.callback(MouseMovedEvent(res));
	});

	glfwSetScrollCallback(window,
	[](GLFWwindow* window, double x, double y) {
		auto& data = *(WindowData*)glfwGetWindowUserPointer(window);
		Ivec2 res(x, y);
		data.callback(MouseScrolledEvent(res));
	});

	glfwSetWindowFocusCallback(window,
	[](GLFWwindow* window, int focus) {
		auto& data = *(WindowData*)glfwGetWindowUserPointer(window);
		data.callback(WindowFocusedEvent(focus));
	});

	glfwSetWindowCloseCallback(window,
	[](GLFWwindow* window) {
		auto& data = *(WindowData*)glfwGetWindowUserPointer(window);
		data.callback(WindowClosedEvent());
	});

	glfwSetWindowSizeCallback(window,
	[](GLFWwindow* window, int width, int height) {
		auto& data = *(WindowData*)glfwGetWindowUserPointer(window);
		data.size = Ivec2(width, height);
		data.callback(WindowResizedEvent(data.size));
	});

	glfwSetWindowPosCallback(window,
	[](GLFWwindow* window, int x, int y) {
		auto& data = *(WindowData*)glfwGetWindowUserPointer(window);
		data.position = Ivec2(x, y);
		data.callback(WindowMovedEvent(data.position));
	});
}

static void SetupWindowHints(std::uint8_t hint)
{
	glfwWindowHint(GLFW_RESIZABLE,		(bool)(hint & WindowResizable));
	glfwWindowHint(GLFW_VISIBLE,		(bool)(hint & WindowVisible));
	glfwWindowHint(GLFW_DECORATED,		(bool)(hint & WindowDecorated));
	glfwWindowHint(GLFW_FOCUSED,		(bool)(hint & WindowFocused));
	glfwWindowHint(GLFW_AUTO_ICONIFY,	(bool)(hint & WindowAutoIconify));
	glfwWindowHint(GLFW_FLOATING,		(bool)(hint & WindowAlwaysOnTop));
	glfwWindowHint(GLFW_MAXIMIZED,		(bool)(hint & WindowMaximized));
}

Window::Window(int width, int height, std::string const& title, std::uint8_t hint)
{
	data.size = Ivec2(width, height);
	data.title = title;
	data.active = true;
	data.has_vsync = false;
	data.callback = [&](const Event& e) -> void { if (EventCast<WindowClosedEvent>(e)) Close(); };
	data.viewport = { 0.0f, 0.0f, width, height };
	destruct_this = true;

	InitGL();
	SetupWindowHints(hint);
	window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
	GLFWwindow* glwin = (GLFWwindow*) window;
	glfwMakeContextCurrent(glwin);
	gladLoadGL();
	glfwSetWindowUserPointer(glwin, &data);
	glViewport(0, 0, width, height);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	CreateWindowEventCallback(glwin);
}

Window::Window(Monitor monitor, std::uint8_t hint)
{
	data.size = monitor.Size();
	data.title = "";
	data.active = true;
	data.callback = [&](const Event& e) -> void { if (EventCast<WindowClosedEvent>(e)) Close(); };
	data.viewport = { 0.0f, 0.0f, monitor.Size()[0], monitor.Size()[1] };
	destruct_this = true;

	InitGL();
	SetupWindowHints(hint);
	window = glfwCreateWindow(monitor.Size()[0], monitor.Size()[1], "", (GLFWmonitor*) monitor.monitor, nullptr);
	GLFWwindow* glwin = (GLFWwindow*) window;
	glfwMakeContextCurrent(glwin);
	gladLoadGL();
	glfwSetWindowUserPointer(glwin, &data);
	glViewport(0, 0, monitor.Size()[0], monitor.Size()[1]);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	CreateWindowEventCallback(glwin);
}

Window::Window(Window&& window) noexcept
	: window(window.window), data(window.data), destruct_this(true)
{
	window.destruct_this = false;
}

Window::~Window()
{
	if (destruct_this)
		glfwDestroyWindow((GLFWwindow*) window);
}

static Window* binded_window = nullptr;

void Window::Bind(Window* window)
{
	if (binded_window == window) return;
	if (window) glfwMakeContextCurrent((GLFWwindow*) window->window);
	else glfwMakeContextCurrent(nullptr);
}

Window* Window::Current()
{
	return binded_window;
}

void Window::Bind()
{
	Bind(this);
}

void Window::Update()
{
	glfwSwapBuffers((GLFWwindow*) window);
	glfwPollEvents();
}

void Window::Close()
{
	data.active = false;
}

void Window::Clear(Fvec4 color)
{
	Bind(this);
	glClearColor(color[0], color[1], color[2], color[3]);
	GLbitfield bf = GL_COLOR_BUFFER_BIT;
	if (Capabilities() & DepthTest) bf |= GL_DEPTH_BUFFER_BIT;
	if (Capabilities() & StencilTest) bf |= GL_STENCIL_BUFFER_BIT;
	glClear(bf);
}

void Window::Position(Ivec2 position)
{
	glfwSetWindowPos((GLFWwindow*) window, position[0], position[1]);
	data.position = position;
}

void Window::Size(Ivec2 size)
{
	glfwSetWindowSize((GLFWwindow*)window, size[0], size[1]);
	data.size = size;
}

void Window::Title(std::string const& title)
{
	glfwSetWindowTitle((GLFWwindow*) window, title.c_str());
	data.title = title;
}

void Window::Viewport(Ivec4 viewport)
{
	glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
	data.viewport = viewport;
}

void Window::Vsync(bool enable)
{
	glfwSwapInterval(enable);
	data.has_vsync = enable;
}

void Window::EventCallback(std::function<void(Window&, Event const&)> const& callback)
{
	data.callback = [&, callback](Event const& e) -> void { callback(*this, e); };
}

bool Window::IsKeyPressed(KeyCode keycode) const
{
	return glfwGetKey((GLFWwindow*) window, keycode);
}

bool Window::IsMouseButtonPressed(MouseButton button) const
{
	return glfwGetMouseButton((GLFWwindow*) window, button);
}

Ivec2 Window::MousePosition() const
{
	double x, y;
	glfwGetCursorPos((GLFWwindow*) window, &x, &y);
	return Ivec2(x, y);
}

#define MATHYW_ENABLE_TEST(cap, oglcap) if (caps & cap) glEnable(oglcap)
#define MATHYW_DISABLE_TEST(cap, oglcap) if (caps & cap) glDisable(oglcap)

static std::uint32_t opengl_capibilities;

void Enable(unsigned caps)
{
	opengl_capibilities |= caps;
	MATHYW_ENABLE_TEST(Blend, GL_BLEND);
	MATHYW_ENABLE_TEST(DepthTest, GL_DEPTH_TEST);
	MATHYW_ENABLE_TEST(CullFace, GL_CULL_FACE);
	MATHYW_ENABLE_TEST(StencilTest, GL_STENCIL_TEST);
	MATHYW_ENABLE_TEST(ScissorTest, GL_SCISSOR_TEST);
}

void Disable(unsigned caps)
{
	opengl_capibilities &= ~caps;
	MATHYW_DISABLE_TEST(Blend, GL_BLEND);
	MATHYW_DISABLE_TEST(DepthTest, GL_DEPTH_TEST);
	MATHYW_DISABLE_TEST(CullFace, GL_CULL_FACE);
	MATHYW_DISABLE_TEST(StencilTest, GL_STENCIL_TEST);
	MATHYW_DISABLE_TEST(ScissorTest, GL_SCISSOR_TEST);
}

std::uint32_t Capabilities()
{
	return opengl_capibilities;
}

#undef MATHYW_ENABLE_TEST
#undef MATHYW_DISABLE_TEST

} // !Mathyw