#pragma once

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image.h>
#include <tiny_gltf.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <set>
#include <vector>
#include <mesh.hpp>
#include <shader.hpp>

uint32_t generateTexture(const tinygltf::Image &image);

struct Model {
    // model data
    std::unordered_map<int32_t, uint32_t> textures_loaded;  // stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
    std::vector<Mesh> meshes;
    // std::string directory;

    Model(std::string const &path);

    void Draw(Shader &shader);

    void loadModel(std::string const &path);

    Mesh processMesh(const tinygltf::Model &model, const tinygltf::Mesh &mesh);

    // checks all material textures of a given type and loads the textures if they're not loaded yet.
    // the required info is returned as a Texture struct.
    // std::vector<Texture> loadMaterialTextures(const tinygltf::Material &material, const tinygltf::Model &model, std::string typeName);
};
