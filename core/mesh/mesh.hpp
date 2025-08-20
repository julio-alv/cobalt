#include <glad/glad.h>  // holds all OpenGL type declarations

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>
#include <vector>

#include <shader.hpp>

// #define MAX_BONE_INFLUENCE 4

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

struct Mesh {
    std::vector<Vertex> _vertices;
    std::vector<uint32_t> _indices;
    std::vector<uint32_t> _textures;
    uint32_t VAO, VBO, EBO;

    Mesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices, std::vector<uint32_t> textures);

    void Draw(Shader &shader);

    void setupMesh();
};