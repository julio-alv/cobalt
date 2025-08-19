#include "model.hpp"

Model::Model(std::string const &path) {
    loadModel(path);
}

void Model::Draw(Shader &shader) {
    for (unsigned int i = 0; i < meshes.size(); i++)
        meshes[i].Draw(shader);
}

void Model::loadModel(std::string const &path) {
    tinygltf::Model model;
    tinygltf::TinyGLTF loader;
    std::string err, warn;

    bool ret = loader.LoadASCIIFromFile(&model, &err, &warn, path);
    if (!ret) {
        throw std::runtime_error("Failed to load glTF: " + path + "\n" + err);
    }

    processNode(model);
}

void Model::processNode(const tinygltf::Model &model) {
    for (const auto &mesh : model.meshes)
        meshes.push_back(processMesh(model, mesh));
}

Mesh Model::processMesh(const tinygltf::Model &model, const tinygltf::Mesh &mesh) {
    // data to fill
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    std::vector<Texture> textures;

    size_t vertexCount;
    const float *positions;
    const float *normals;
    const float *texs;

    for (const auto &primitive : mesh.primitives) {
        {
            const auto &accessor = model.accessors[primitive.attributes.at("POSITION")];
            const auto &view = model.bufferViews[accessor.bufferView];
            const auto &buffer = model.buffers[view.buffer];
            positions = reinterpret_cast<const float *>(&buffer.data[view.byteOffset + accessor.byteOffset]);
            vertexCount = accessor.count;
        }
        {
            const auto &accessor = model.accessors[primitive.attributes.at("NORMAL")];
            const auto &view = model.bufferViews[accessor.bufferView];
            const auto &buffer = model.buffers[view.buffer];
            normals = reinterpret_cast<const float *>(&buffer.data[view.byteOffset + accessor.byteOffset]);
        }
        {
            const auto &accessor = model.accessors[primitive.attributes.at("TEXCOORD_0")];
            const auto &view = model.bufferViews[accessor.bufferView];
            const auto &buffer = model.buffers[view.buffer];
            texs = reinterpret_cast<const float *>(&buffer.data[view.byteOffset + accessor.byteOffset]);
        }
        {
            const auto &accessor = model.accessors.at(primitive.indices);
            const auto &view = model.bufferViews[accessor.bufferView];
            const auto &buffer = model.buffers[view.buffer];

            const uint8_t *dataPtr =
                buffer.data.data() + view.byteOffset + accessor.byteOffset;

            size_t stride = accessor.ByteStride(view);
            if (stride == 0) {
                stride = tinygltf::GetComponentSizeInBytes(
                    accessor.componentType);
            }

            indices.reserve(accessor.count);
            for (size_t i = 0; i < accessor.count; i++) {
                uint32_t index = 0;
                switch (accessor.componentType) {
                    case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE: {
                        index = *(reinterpret_cast<const uint8_t *>(
                            dataPtr + i * stride));
                        break;
                    }
                    case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT: {
                        index = *(reinterpret_cast<const uint16_t *>(
                            dataPtr + i * stride));
                        break;
                    }
                    case TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT: {
                        index = *(reinterpret_cast<const uint32_t *>(
                            dataPtr + i * stride));
                        break;
                    }
                    default:
                        std::cerr << "Unsupported index type: "
                                  << accessor.componentType << std::endl;
                        break;
                }
                indices.push_back(index);
            }
        }
        {
            const auto &mat = model.materials[primitive.material];

            // Base color → diffuse
            if (mat.pbrMetallicRoughness.baseColorTexture.index >= 0) {
                int texIndex = mat.pbrMetallicRoughness.baseColorTexture.index;
                const auto &tex = model.textures[texIndex];
                if (tex.source >= 0) {
                    textures.push_back(loadTextureFromImage(
                        model.images[tex.source], "texture_diffuse"));
                }
            }

            // Normal map
            if (mat.normalTexture.index >= 0) {
                int texIndex = mat.normalTexture.index;
                const auto &tex = model.textures[texIndex];
                if (tex.source >= 0) {
                    textures.push_back(loadTextureFromImage(
                        model.images[tex.source], "texture_normal"));
                }
            }

            // Metallic-Roughness → treat as specular
            if (mat.pbrMetallicRoughness.metallicRoughnessTexture.index >= 0) {
                int texIndex =
                    mat.pbrMetallicRoughness.metallicRoughnessTexture.index;
                const auto &tex = model.textures[texIndex];
                if (tex.source >= 0) {
                    textures.push_back(loadTextureFromImage(
                        model.images[tex.source], "texture_specular"));
                }
            }

            // Occlusion map → treat as height
            if (mat.occlusionTexture.index >= 0) {
                int texIndex = mat.occlusionTexture.index;
                const auto &tex = model.textures[texIndex];
                if (tex.source >= 0) {
                    textures.push_back(loadTextureFromImage(
                        model.images[tex.source], "texture_height"));
                }
            }
        }
    }

    vertices.reserve(vertexCount);

    for (size_t i = 0; i < vertexCount; i++) {
        Vertex vertex;
        vertex.Position.x = positions[i * 3 + 0];
        vertex.Position.y = positions[i * 3 + 1];
        vertex.Position.z = positions[i * 3 + 2];

        vertex.Normal.x = normals[i * 3 + 0];
        vertex.Normal.y = normals[i * 3 + 1];
        vertex.Normal.z = normals[i * 3 + 2];

        vertex.TexCoords.x = texs[i * 2 + 0];
        vertex.TexCoords.y = texs[i * 2 + 1];

        vertices.push_back(vertex);
    }

    return Mesh(vertices, indices, textures);
}

Texture loadTextureFromImage(const tinygltf::Image &image,
                             const std::string &typeName) {
    GLuint texId;
    glGenTextures(1, &texId);
    glBindTexture(GL_TEXTURE_2D, texId);

    GLenum format = GL_RGBA;
    if (image.component == 1)
        format = GL_RED;
    else if (image.component == 3)
        format = GL_RGB;
    else if (image.component == 4)
        format = GL_RGBA;

    glTexImage2D(GL_TEXTURE_2D, 0, format, image.width, image.height, 0,
                 format, GL_UNSIGNED_BYTE, image.image.data());
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    Texture texture;
    texture.id = texId;
    texture.type = typeName;
    texture.path = image.uri;  // may be empty if embedded

    return texture;
}

// uint32_t TextureFromFile(const char *path, const std::string &directory, bool gamma) {
//     std::string filename = std::string(path);
//     filename = directory + '/' + filename;

//     uint32_t textureID;
//     glGenTextures(1, &textureID);

//     int width, height, nrComponents;
//     stbi_set_flip_vertically_on_load(true);
//     uint8_t *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
//     stbi_set_flip_vertically_on_load(false);

//     if (data) {
//         GLenum format;
//         if (nrComponents == 1)
//             format = GL_RED;
//         else if (nrComponents == 3)
//             format = GL_RGB;
//         else if (nrComponents == 4)
//             format = GL_RGBA;

//         glBindTexture(GL_TEXTURE_2D, textureID);
//         glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
//         glGenerateMipmap(GL_TEXTURE_2D);

//         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

//         stbi_image_free(data);
//     } else {
//         std::cout << "Texture failed to load at path: " << path << std::endl;
//         stbi_image_free(data);
//     }

//     return textureID;
// }

// std::vector<Texture> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName) {
//     std::vector<Texture> textures;
//     for (uint32_t i = 0; i < mat->GetTextureCount(type); i++) {
//         aiString str;
//         mat->GetTexture(type, i, &str);
//         // check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
//         bool skip = false;
//         for (uint32_t j = 0; j < textures_loaded.size(); j++) {
//             if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0) {
//                 textures.push_back(textures_loaded[j]);
//                 skip = true;  // a texture with the same filepath has already been loaded, continue to next one. (optimization)
//                 break;
//             }
//         }
//         if (!skip) {  // if texture hasn't been loaded already, load it
//             Texture texture;
//             texture.id = TextureFromFile(str.C_Str(), this->directory);
//             texture.type = typeName;
//             texture.path = str.C_Str();
//             textures.push_back(texture);
//             textures_loaded.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecessary load duplicate textures.
//         }
//     }
//     return textures;
// }