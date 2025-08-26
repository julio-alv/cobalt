#include <glad/glad.h>
#include <fstream>
#include <iostream>
#include <sstream>

#include "shader.h"

Shader::Shader(const std::string &vertPath, const std::string &fragPath)
    : _id(0)
{
    std::stringstream vertBuff;
    std::stringstream fragBuff;
    vertBuff << std::ifstream(vertPath).rdbuf();
    fragBuff << std::ifstream(fragPath).rdbuf();
    auto vertStr = vertBuff.str();
    auto fragStr = fragBuff.str();

    createShader(vertStr, fragStr);
}

Shader::~Shader()
{
    glDeleteProgram(_id);
}

void Shader::Bind() const
{
    glUseProgram(_id);
}

void Shader::Unbind() const
{
    glUseProgram(0);
}

void Shader::SetUniform1i(const std::string &name, std::int32_t value)
{
    glUniform1i(getUniformLocation(name), value);
}

void Shader::SetUniform1f(const std::string &name, float value)
{
    glUniform1f(getUniformLocation(name), value);
}

void Shader::SetUniform4f(const std::string &name, float f0, float f1, float f2, float f3)
{
    glUniform4f(getUniformLocation(name), f0, f1, f2, f3);
}

std::int32_t Shader::getUniformLocation(const std::string &name)
{
    if (_cache.find(name) != _cache.end())
        return _cache[name];

    std::int32_t location = glGetUniformLocation(_id, name.c_str());
    if (location == -1)
        std::printf("Warning, uniform '%s' does not exist\n", name.c_str());

    _cache[name] = location;
    return location;
}

void Shader::createShader(const std::string &vert, const std::string &frag)
{
    _id = glCreateProgram();

    std::uint32_t vs = compileShader(vert, GL_VERTEX_SHADER);
    std::uint32_t fs = compileShader(frag, GL_FRAGMENT_SHADER);

    glAttachShader(_id, vs);
    glAttachShader(_id, fs);
    glLinkProgram(_id);
    glValidateProgram(_id);

    glDetachShader(_id, vs);
    glDetachShader(_id, fs);
    glDeleteShader(vs);
    glDeleteShader(fs);
}

std::uint32_t Shader::compileShader(const std::string &source, std::uint32_t type)
{
    std::uint32_t id = glCreateShader(type);
    auto *src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    std::int32_t result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (!result)
    {
        std::int32_t length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        auto message = static_cast<char *>(alloca(length * sizeof(char)));
        glGetShaderInfoLog(id, length, &length, message);
        std::printf("shader compilation failed!: %s\n", message);
        glDeleteShader(id);
        return 0;
    }

    return id;
}
