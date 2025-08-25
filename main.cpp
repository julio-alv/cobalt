#include <glad/glad.h>

#include "window.h"
#include "renderer.h"
#include "texture.h"

constexpr std::int32_t WIDTH = 1200;
constexpr std::int32_t HEIGHT = 900;

int main()
{
    Window win{WIDTH, HEIGHT, "Cobalt"};
    win.SwapInterval(1); // set the swap interval to the monitor's refresh rate

    float vertices[] = {
        -.5f, -.5f, 0.0f, 0.0f,
        0.5f, -.5f, 1.0f, 0.0f,
        0.5f, 0.5f, 1.0f, 1.0f,
        -.5f, 0.5f, 0.0f, 1.0f};

    std::uint32_t indices[]{
        0, 1, 2,
        2, 3, 0};

    VertexArray va;
    VertexBuffer vb{vertices, sizeof(vertices)};
    VertexLayout layout;
    layout.Push<float>(2);
    layout.Push<float>(2);
    va.AddBuffer(vb, layout);
    IndexBuffer ib{indices, 6};

    Shader shader{"../res/shaders/basic.vert", "../res/shaders/basic.frag"};

    Texture texture{"../res/textures/skybox/front.jpg"};
    texture.Bind();
    shader.SetUniform1i("u_Texture", 0);

    Renderer renderer;

    while (!win.ShouldClose())
    {
        renderer.Clear();
        renderer.Draw(va, ib, shader);

        win.SwapBuffers();
        win.PollEvents();
    }
}
