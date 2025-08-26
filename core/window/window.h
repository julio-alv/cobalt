#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <functional>

class Window
{
    GLFWwindow *_window;
    double _lastTime = 0;

public:
    explicit Window(std::int32_t width, std::int32_t height, const std::string &name);
    ~Window();

    std::int32_t ShouldClose();
    void SwapBuffers();
    void PollEvents();
    void WaitEvents();
    float GetDeltaTime();
    void SwapInterval(std::int32_t i);
    void SetResizeCallback(void (*callback)(GLFWwindow *window, std::int32_t width, std::int32_t height));
};