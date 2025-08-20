#include "model.hpp"

Model::Model(std::string const &path) { loadModel(path); }

void Model::Draw(Shader &shader)
{
    for (unsigned int i = 0; i < meshes.size(); i++)
        meshes[i].Draw(shader);
}

void Model::loadModel(std::string const &path)
{
    tinygltf::Model model;
    tinygltf::TinyGLTF loader;
    std::string err, warn;

    bool ret = loader.LoadASCIIFromFile(&model, &err, &warn, path);
    if (!ret)
    {
        throw std::runtime_error("Failed to load glTF: " + path + "\n" + err);
    }

    for (const auto &mesh : model.meshes)
        meshes.push_back(processMesh(model, mesh));
}

Mesh Model::processMesh(const tinygltf::Model &model, const tinygltf::Mesh &mesh)
{
    // data to fill
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    std::vector<uint32_t> textures;

    size_t vertexCount;
    const float *positions;
    const float *normals;
    const float *texs;

    for (const auto &primitive : mesh.primitives)
    {
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

            const uint8_t *dataPtr = buffer.data.data() + view.byteOffset + accessor.byteOffset;

            size_t stride = accessor.ByteStride(view);
            if (stride == 0)
                stride = tinygltf::GetComponentSizeInBytes(accessor.componentType);

            indices.reserve(accessor.count);
            for (size_t i = 0; i < accessor.count; i++)
            {
                uint32_t index = 0;
                switch (accessor.componentType)
                {
                case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE:
                {
                    index = *(reinterpret_cast<const uint8_t *>(dataPtr + i * stride));
                    break;
                }
                case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT:
                {
                    index = *(reinterpret_cast<const uint16_t *>(dataPtr + i * stride));
                    break;
                }
                case TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT:
                {
                    index = *(reinterpret_cast<const uint32_t *>(dataPtr + i * stride));
                    break;
                }
                default:
                    std::cerr << "Unsupported index type: " << accessor.componentType << std::endl;
                    break;
                }
                indices.push_back(index);
            }
        }
        {
            if (model.textures.size() == 0)
            {
                continue;
            }
            const auto &mat = model.materials[0];
            int32_t diffuse_index = mat.pbrMetallicRoughness.baseColorTexture.index;
            int32_t specular_index = mat.pbrMetallicRoughness.metallicRoughnessTexture.index;
            int32_t normal_index = mat.normalTexture.index;

            const tinygltf::Texture &diffuse_texture = model.textures[diffuse_index];
            const tinygltf::Texture &specular_texture = model.textures[specular_index];
            const tinygltf::Texture &normal_texture = model.textures[normal_index];

            int32_t diffuse_id = diffuse_texture.source;
            int32_t specular_id = specular_texture.source;
            int32_t normal_id = normal_texture.source;

            const tinygltf::Image &diffuse = model.images[diffuse_id];
            const tinygltf::Image &specular = model.images[specular_id];
            const tinygltf::Image &normal = model.images[normal_id];

            uint32_t diff;
            uint32_t spec;
            uint32_t norm;

            if (!textures_loaded.contains(diffuse_id))
            {
                diff = generateTexture(diffuse);
                textures_loaded.insert({diffuse_id, diff});
            }
            else
            {
                diff = textures_loaded.at(diffuse_id);
            }

            if (!textures_loaded.contains(specular_id))
            {
                spec = generateTexture(specular);
                textures_loaded.insert({specular_id, diff});
            }
            else
            {
                spec = textures_loaded.at(specular_id);
            }

            if (!textures_loaded.contains(normal_id))
            {
                norm = generateTexture(normal);
                textures_loaded.insert({normal_id, diff});
            }
            else
            {
                norm = textures_loaded.at(normal_id);
            }

            textures.push_back(diff);
            textures.push_back(spec);
            textures.push_back(norm);
        }
    }

    vertices.reserve(vertexCount);
    for (size_t i = 0; i < vertexCount; i++)
    {
        Vertex v;
        v.Position.x = positions[i * 3 + 0];
        v.Position.y = positions[i * 3 + 1];
        v.Position.z = positions[i * 3 + 2];

        v.Normal.x = normals[i * 3 + 0];
        v.Normal.y = normals[i * 3 + 1];
        v.Normal.z = normals[i * 3 + 2];

        v.TexCoords.x = texs[i * 2 + 0];
        v.TexCoords.y = texs[i * 2 + 1];

        vertices.push_back(v);
    }

    return Mesh(vertices, indices, textures);
}

uint32_t generateTexture(const tinygltf::Image &image)
{
    uint32_t id;
    glGenTextures(1, &id);

    uint32_t format;
    if (image.component == 1)
        format = GL_RED;
    else if (image.component == 3)
        format = GL_RGB;
    else if (image.component == 4)
        format = GL_RGBA;

    glBindTexture(GL_TEXTURE_2D, id);
    glTexImage2D(GL_TEXTURE_2D, 0, format, image.width, image.height, 0, format, GL_UNSIGNED_BYTE, image.image.data());
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    return id;
}