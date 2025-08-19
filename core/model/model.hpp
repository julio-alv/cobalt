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

    Mesh processMesh(const tinygltf::Model &model, const tinygltf::Mesh &mesh);

    // checks all material textures of a given type and loads the textures if they're not loaded yet.
    // the required info is returned as a Texture struct.
    // std::vector<Texture> loadMaterialTextures(const tinygltf::Material &material, const tinygltf::Model &model, std::string typeName);
};
