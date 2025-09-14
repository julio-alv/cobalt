#define GLM_ENABLE_EXPERIMENTAL
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <tiny_gltf.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "window.h"
#include "renderer.h"
#include "texture.h"
#include "transform.h"

constexpr std::int32_t WIDTH = 800;
constexpr std::int32_t HEIGHT = 600;

void resize(GLFWwindow *win, std::int32_t width, std::int32_t height)
{
    glViewport(0, 0, width, height);
}

inline glm::quat fromEulerYXZ(const glm::vec3 &eulerAngles)
{
    glm::quat yaw = glm::angleAxis(glm::radians(eulerAngles.y), glm::vec3(0, 1, 0));
    glm::quat pitch = glm::angleAxis(glm::radians(eulerAngles.x), glm::vec3(1, 0, 0));
    glm::quat roll = glm::angleAxis(glm::radians(eulerAngles.z), glm::vec3(0, 0, 1));

    return yaw * pitch * roll;
}

int main()
{
    Window win{WIDTH, HEIGHT, "Cobalt"};
    win.SwapInterval(1); // set the swap interval to the monitor's refresh rate
    win.SetResizeCallback(resize);

    float vertices[] = {
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,

        -0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,

        0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

        -0.5f, 0.5f, -0.5f, 0.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f};

    std::uint32_t indices[] = {
        0, 1, 2, 2, 3, 0,
        4, 5, 6, 6, 7, 4,
        8, 9, 10, 10, 11, 8,
        12, 13, 14, 14, 15, 12,
        16, 17, 18, 18, 19, 16,
        20, 21, 22, 22, 23, 20};

    VertexArray va;
    VertexBuffer vb{vertices, sizeof(vertices)};
    VertexLayout layout;
    layout.Push<float>(3);
    layout.Push<float>(2);
    va.AddBuffer(vb, layout);
    IndexBuffer ib{indices, 36};

    glm::vec3 position = glm::vec3(0.0f, 0.0f, -2.0f);
    glm::vec3 eulerAngles = glm::vec3(0.0f);
    glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);

    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::perspective(glm::radians(90.0f), (float)WIDTH / (float)HEIGHT, 0.1f, -100.0f);

    Shader shader{"../res/shaders/basic.vert", "../res/shaders/basic.frag"};

    Texture texture{"../res/textures/skybox/front.jpg"};
    texture.Bind();
    shader.SetUniform1i("u_Texture", 0);

    Renderer renderer;
    glEnable(GL_DEPTH_TEST);
    // glEnable(GL_CULL_FACE);

    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    ImGui_ImplGlfw_InitForOpenGL(win.GetWindow(), true);
    ImGui_ImplOpenGL3_Init("#version 410");

    ImGui::StyleColorsDark();

    while (!win.ShouldClose())
    {
        win.PollEvents();

        renderer.Clear();
        glm::quat orientation = fromEulerYXZ(eulerAngles);
        glm::mat4 model = glm::translate(glm::mat4(1.0f), position) * glm::toMat4(orientation) * glm::scale(glm::mat4(1.0f), scale);

        shader.Bind();
        shader.SetUniformMat4f("u_Model", model);
        shader.SetUniformMat4f("u_View", view);
        shader.SetUniformMat4f("u_Projection", projection);
        renderer.Draw(va, ib, shader);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        {
            ImGui::Begin("Panel");

            ImGui::SliderFloat3("position", &position[0], -10.0f, 10.0f);
            ImGui::SliderFloat3("rotation", &eulerAngles[0], -180.0f, 180.0f);
            ImGui::SliderFloat3("scale", &scale[0], -10.0f, 10.0f);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
            ImGui::End();
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        win.SwapBuffers();
    }

    ImGui_ImplGlfw_Shutdown();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui::DestroyContext();
}
