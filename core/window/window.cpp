#include <cassert>

#include "window.h"

Window::Window(std::int32_t width, std::int32_t height, const std::string &name)
{
    assert(glfwInit());
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    _window = glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr);
    assert(_window);

    glfwMakeContextCurrent(_window);
    assert(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress));
}

Window::~Window()
{
    glfwDestroyWindow(_window);
    glfwTerminate();
}

std::int32_t Window::ShouldClose() { return glfwWindowShouldClose(_window); }
void Window::SwapBuffers() { glfwSwapBuffers(_window); }
void Window::PollEvents() { glfwPollEvents(); }
void Window::WaitEvents() { glfwWaitEvents(); }
void Window::SwapInterval(std::int32_t i) { glfwSwapInterval(i); }