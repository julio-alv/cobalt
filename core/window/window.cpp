#include "window.hpp"

Window::Window(int32_t width, int32_t height, std::string_view name)
{
    assert(glfwInit());
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    m_Window = glfwCreateWindow(width, height, name.cbegin(), nullptr, nullptr);
    assert(m_Window);

    glfwMakeContextCurrent(m_Window);
    assert(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress));
}

Window::~Window()
{
    glfwDestroyWindow(m_Window);
    glfwTerminate();
}

bool Window::ShouldClose() { return glfwWindowShouldClose(m_Window); }
void Window::SwapBuffers() { glfwSwapBuffers(m_Window); }
void Window::PollEvents() { glfwPollEvents(); }
void Window::WaitEvents() { glfwWaitEvents(); }