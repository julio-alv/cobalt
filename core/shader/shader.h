#pragma once

#include <string>
#include <unordered_map>

class Shader
{
    unsigned int _id;
    std::unordered_map<std::string, unsigned int> _cache;

public:
    Shader(const std::string &vertPath, const std::string &fragPath);
    ~Shader();

    void Bind() const;
    void Unbind() const;

    void SetUniform4f(const std::string &name, float f0, float f1, float f2, float f3);

private:
    unsigned int getUniformLocation(const std::string &name);
    void createShader(const std::string &vert, const std::string &frag);
    unsigned int compileShader(const std::string &source, unsigned int type);
};
