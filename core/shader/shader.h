#pragma once

#include <glm/glm.hpp>

#include <string>
#include <unordered_map>

class Shader
{
    std::uint32_t _id;
    std::unordered_map<std::string, std::int32_t> _cache;

public:
    Shader(const std::string &vertPath, const std::string &fragPath);
    ~Shader();

    void Bind() const;
    void Unbind() const;

    void SetUniform1i(const std::string &name, std::int32_t value);
    void SetUniform1f(const std::string &name, float value);
    void SetUniform4f(const std::string &name, float f0, float f1, float f2, float f3);
    void SetUniformMat4f(const std::string &name, const glm::mat4 &matrix);

private:
    std::int32_t getUniformLocation(const std::string &name);
    void createShader(const std::string &vert, const std::string &frag);
    std::uint32_t compileShader(const std::string &source, std::uint32_t type);
};
