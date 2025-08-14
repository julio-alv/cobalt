#include "shader.hpp"

Shader::Shader(const char *vertPath, const char *fragPath) {
    std::stringstream vertBuff;
    std::stringstream fragBuff;
    vertBuff << std::ifstream(vertPath).rdbuf();
    fragBuff << std::ifstream(fragPath).rdbuf();
    std::string vertStr = vertBuff.str();
    std::string fragStr = fragBuff.str();
    auto vertSource = vertStr.c_str();
    auto fragSource = fragStr.c_str();

    uint32_t vert = glCreateShader(GL_VERTEX_SHADER);
    uint32_t frag = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(vert, 1, &vertSource, nullptr);
    glShaderSource(frag, 1, &fragSource, nullptr);
    glCompileShader(vert);
    checkCompileErrors(vert, "VERTEX");
    glCompileShader(frag);
    checkCompileErrors(frag, "FRAGMENT");

    this->id = glCreateProgram();

    glAttachShader(this->id, vert);
    glAttachShader(this->id, frag);
    glLinkProgram(this->id);
    checkCompileErrors(this->id, "PROGRAM");
    glDeleteShader(vert);
    glDeleteShader(frag);
}

void Shader::checkCompileErrors(unsigned int shader, std::string type) {
    int success;
    char infoLog[1024];
    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::printf("ERROR::SHADER_COMPILATION_ERROR of type: %s ERROR %s\n", type.c_str(), infoLog);
        }
    } else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::printf("ERROR::PROGRAM_LINKING_ERROR of type: %s ERROR %s\n", type.c_str(), infoLog);
        }
    }
}

Shader::~Shader() { glDeleteProgram(this->id); }

void Shader::use() { glUseProgram(this->id); }

void Shader::setBool(const std::string &name, bool value) const {
    glUniform1i(glGetUniformLocation(this->id, name.c_str()), (int)value);
}

void Shader::setInt(const std::string &name, int value) const {
    glUniform1i(glGetUniformLocation(this->id, name.c_str()), value);
}

void Shader::setFloat(const std::string &name, float value) const {
    glUniform1f(glGetUniformLocation(this->id, name.c_str()), value);
}

void Shader::setVec2(const std::string &name, const glm::vec2 &value) const {
    glUniform2fv(glGetUniformLocation(this->id, name.c_str()), 1, &value[0]);
}
void Shader::setVec2(const std::string &name, float x, float y) const {
    glUniform2f(glGetUniformLocation(this->id, name.c_str()), x, y);
}

void Shader::setVec3(const std::string &name, const glm::vec3 &value) const {
    glUniform3fv(glGetUniformLocation(this->id, name.c_str()), 1, &value[0]);
}
void Shader::setVec3(const std::string &name, float x, float y, float z) const {
    glUniform3f(glGetUniformLocation(this->id, name.c_str()), x, y, z);
}

void Shader::setVec4(const std::string &name, const glm::vec4 &value) const {
    glUniform4fv(glGetUniformLocation(this->id, name.c_str()), 1, &value[0]);
}
void Shader::setVec4(const std::string &name, float x, float y, float z, float w) const {
    glUniform4f(glGetUniformLocation(this->id, name.c_str()), x, y, z, w);
}

void Shader::setMat2(const std::string &name, const glm::mat2 &mat) const {
    glUniformMatrix2fv(glGetUniformLocation(this->id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setMat3(const std::string &name, const glm::mat3 &mat) const {
    glUniformMatrix3fv(glGetUniformLocation(this->id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setMat4(const std::string &name, const glm::mat4 &mat) const {
    glUniformMatrix4fv(glGetUniformLocation(this->id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
