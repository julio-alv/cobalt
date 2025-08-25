#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>

class Window
{
    GLFWwindow *_window;

public:
    explicit Window(std::int32_t width, std::int32_t height, const std::string &name);
    ~Window();

    std::int32_t ShouldClose();
    void SwapBuffers();
    void PollEvents();
    void WaitEvents();
    void SwapInterval(std::int32_t i);
};