#include <glad/glad.h>

#include "window.h"
#include "shader.h"
#include "indexbuffer.h"
#include "vertexbuffer.h"
#include "vertexarray.h"

constexpr int WIDTH = 1200;
constexpr int HEIGHT = 900;

int main()
{
    Window win{WIDTH, HEIGHT, "Cobalt"};
    win.SwapInterval(1); // set the swap interval to the monitor's refresh rate

    float vertices[] = {
        -.5f, -.5f, 1.0f, 0.0f, 0.0f, 1.0f,
        0.5f, -.5f, 0.0f, 1.0f, 0.0f, 1.0f,
        0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f,
        -.5f, 0.5f, 1.0f, 1.0f, 0.0f, 1.0f};

    unsigned int indices[]{
        0, 1, 2,
        2, 3, 0};

    VertexArray va;
    VertexBuffer vbo{vertices, sizeof(vertices)};
    VertexLayout layout;
    layout.Push<float>(2);
    layout.Push<float>(4);
    va.AddBuffer(vbo, layout);
    IndexBuffer ibo{indices, 6};

    Shader shader{"../res/shaders/basic.vert", "../res/shaders/basic.frag"};

    while (!win.ShouldClose())
    {
        glClear(GL_COLOR_BUFFER_BIT);

        shader.Bind();
        shader.SetUniform4f("u_Color", 0, 1, 0, 1);
        va.Bind();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        win.SwapBuffers();
        win.PollEvents();
    }
}
