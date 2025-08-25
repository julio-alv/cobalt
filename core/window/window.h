#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>

class Window
{
    GLFWwindow *_window;

public:
    explicit Window(int width, int height, const std::string &name);
    ~Window();

    int ShouldClose();
    void SwapBuffers();
    void PollEvents();
    void WaitEvents();
    void SwapInterval(int i);
};