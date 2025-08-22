#include <glad/glad.h>
#include <window.hpp>

constexpr int32_t WIDTH = 1200;
constexpr int32_t HEIGHT = 900;

int main()
{
    Window win{WIDTH, HEIGHT, "Cobalt"};

    while (!win.ShouldClose())
    {
        glClearColor(0.0f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        win.SwapBuffers();
        win.PollEvents();
    }
}
