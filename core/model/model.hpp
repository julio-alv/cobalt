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
#include <map>
#include <vector>
#include <mesh.hpp>
#include <shader.hpp>

// uint32_t TextureFromFile(const char *path, const std::string &directory, bool gamma = false);
Texture loadTextureFromImage(const tinygltf::Image &image, const std::string &typeName);

struct Model {
    // model data
    std::vector<Texture> textures_loaded;  // stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
    std::vector<Mesh> meshes;
    // std::string directory;

    // constructor, expects a filepath to a 3D model.
    Model(std::string const &path);

    // draws the model, and thus all its meshes
    void Draw(Shader &shader);

    // loads a model with tiny_gltf from file and stores the resulting meshes in the meshes vector.
    void loadModel(std::string const &path);

    // processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
    void processNode(const tinygltf::Model &model);

    Mesh processMesh(const tinygltf::Model &model, const tinygltf::Mesh &mesh);

    // checks all material textures of a given type and loads the textures if they're not loaded yet.
    // the required info is returned as a Texture struct.
    // std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
};
