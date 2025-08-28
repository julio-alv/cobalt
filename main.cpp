#define GLM_ENABLE_EXPERIMENTAL
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

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

    glm::vec3 position = glm::vec3(0.0f, 0.0f, -2.0f);
    glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);

    glm::mat4 view = glm::rotate(glm::mat4(1.0f), glm::radians(15.0f), glm::vec3(0, 1, 0));
    glm::mat4 projection = glm::perspective(glm::radians(90.0f), (float)WIDTH / (float)HEIGHT, 0.0f, -100.0f);

    Shader shader{"../res/shaders/basic.vert", "../res/shaders/basic.frag"};

    Texture texture{"../res/textures/skybox/front.jpg"};
    texture.Bind();
    shader.SetUniform1i("u_Texture", 0);

    Renderer renderer;
    // glEnable(GL_DEPTH_TEST);

    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    ImGui_ImplGlfw_InitForOpenGL(win.GetWindow(), true);
    ImGui_ImplOpenGL3_Init("#version 410");

    ImGui::StyleColorsDark();

    while (!win.ShouldClose())
    {
        win.PollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        {
            ImGui::Begin("Panel");

            ImGui::SliderFloat3("position", &position[0], -10.0f, 10.0f);
            // ImGui::SliderFloat3("rotation", &rotation[0], -180.0f, 180.0f);
            ImGui::SliderFloat3("scale", &scale[0], -10.0f, 10.0f);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
            ImGui::End();
        }

        float deltaTime = win.GetDeltaTime();
        renderer.Clear();
        glm::mat4 model = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), scale);

        shader.Bind();
        shader.SetUniformMat4f("u_Model", model);
        shader.SetUniformMat4f("u_View", view);
        shader.SetUniformMat4f("u_Projection", projection);
        renderer.Draw(va, ib, shader);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        win.SwapBuffers();
    }

    ImGui_ImplGlfw_Shutdown();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui::DestroyContext();
}
