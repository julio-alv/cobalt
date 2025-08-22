#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <cassert>

class Window
{
private:
    GLFWwindow *m_Window;

public:
    explicit Window(int32_t width, int32_t height, std::string_view name);
    ~Window();

    bool ShouldClose();
    void SwapBuffers();
    void PollEvents();
    void WaitEvents();
};