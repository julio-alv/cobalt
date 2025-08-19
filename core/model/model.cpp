#include "model.hpp"

Model::Model(std::string const &path) { loadModel(path); }

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

            const uint8_t *dataPtr = buffer.data.data() + view.byteOffset + accessor.byteOffset;

            size_t stride = accessor.ByteStride(view);
            if (stride == 0)
                stride = tinygltf::GetComponentSizeInBytes(accessor.componentType);

            indices.reserve(accessor.count);
            for (size_t i = 0; i < accessor.count; i++) {
                uint32_t index = 0;
                switch (accessor.componentType) {
                    case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE: {
                        index = *(reinterpret_cast<const uint8_t *>(dataPtr + i * stride));
                        break;
                    }
                    case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT: {
                        index = *(reinterpret_cast<const uint16_t *>(dataPtr + i * stride));
                        break;
                    }
                    case TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT: {
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
    }

    vertices.reserve(vertexCount);
    for (size_t i = 0; i < vertexCount; i++) {
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
