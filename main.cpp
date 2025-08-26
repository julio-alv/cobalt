#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "window.h"
#include "renderer.h"
#include "texture.h"

constexpr std::int32_t WIDTH = 800;
constexpr std::int32_t HEIGHT = 600;

void resize(GLFWwindow *win, std::int32_t width, std::int32_t height)
{
    glViewport(0, 0, width, height);
}

int main()
{
    Window win{WIDTH, HEIGHT, "Cobalt"};
    win.SwapInterval(1); // set the swap interval to the monitor's refresh rate
    win.SetResizeCallback(resize);

    float vertices[] = {
        -.5f, -.5f, -0.0f, 0.0f, 0.0f,
        0.5f, -.5f, -0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.0f, 1.0f, 1.0f,
        -.5f, 0.5f, -0.0f, 0.0f, 1.0f};

    std::uint32_t indices[]{
        0, 1, 2,
        2, 3, 0};

    VertexArray va;
    VertexBuffer vb{vertices, sizeof(vertices)};
    VertexLayout layout;
    layout.Push<float>(3);
    layout.Push<float>(2);
    va.AddBuffer(vb, layout);
    IndexBuffer ib{indices, 6};

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);

    projection = glm::perspective(glm::radians(90.0f), (float)WIDTH / (float)HEIGHT, 0.0f, -100.0f);
    view = glm::rotate(view, glm::radians(15.0f), glm::vec3(0, 1, 0));
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.0f));

    Shader shader{"../res/shaders/basic.vert", "../res/shaders/basic.frag"};
    shader.Bind();
    shader.SetUniformMat4f("u_Projection", projection);
    shader.SetUniformMat4f("u_View", view);
    shader.SetUniformMat4f("u_Model", model);

    Texture texture{"../res/textures/skybox/front.jpg"};
    texture.Bind();
    shader.SetUniform1i("u_Texture", 0);

    Renderer renderer;
    // glEnable(GL_DEPTH_TEST);

    while (!win.ShouldClose())
    {
        float deltaTime = win.GetDeltaTime();
        renderer.Clear();
        shader.Bind();
        model = glm::rotate(model, glm::radians(90.f) * deltaTime, glm::vec3(0.0f, 1.0f, 0.0f));
        shader.SetUniformMat4f("u_Model", model);
        shader.SetUniformMat4f("u_View", view);
        shader.SetUniformMat4f("u_Projection", projection);
        renderer.Draw(va, ib, shader);

        win.SwapBuffers();
        win.PollEvents();
    }
}
