#include "Shader.h"

#include <glad/glad.h>
#include <utils/Log.h>
#include <utils/FileUtils.h>

#include <fstream>
#include <sstream>
#include <string>

Shader* Shader::load_shader(const char* vertexFile, const char* fragmentFile) {
    std::string vertexCode;
    std::string fragmentCode;

    try {
        vertexCode = get_fileContents(vertexFile);
        fragmentCode = get_fileContents(fragmentFile);

        //LINFO(vertexCode);
        //LINFO(fragmentCode);
    }
    catch (std::ifstream::failure& e) {
        LCRITICAL("Shader file read error:\n{}", e.what());
        return nullptr;
    }

    return new Shader(vertexCode.c_str(), fragmentCode.c_str());
}

bool Shader::create_shader(const char* code, const ShaderType shaderType, unsigned int& shaderUUID)
{
    shaderUUID = glCreateShader(shader_type_to_component_type(shaderType));
    glShaderSource(shaderUUID, 1, &code, nullptr);
    glCompileShader(shaderUUID);

    GLint success;
    glGetShaderiv(shaderUUID, GL_COMPILE_STATUS, &success);
    if (success == GL_FALSE)
    {
        char info_log[1024];
        glGetShaderInfoLog(shaderUUID, 1024, nullptr, info_log);

        LCRITICAL("Shader compilation error:\n{}", info_log);
        return false;
    }
    
    return true;
}

Shader::Shader(const char* vertexCode, const char* fragmentCode)
{
    GLuint vertex_shader_id = 0;
    if (!create_shader(vertexCode, ShaderType::Vertex, vertex_shader_id))
    {
        LCRITICAL("VERTEX SHADER: compile-time error!");
        glDeleteShader(vertex_shader_id);
        return;
    }

    GLuint fragment_shader_id = 0;
    if (!create_shader(fragmentCode, ShaderType::Fragment, fragment_shader_id))
    {
        LCRITICAL("FRAGMENT SHADER: compile-time error!");
        glDeleteShader(vertex_shader_id);
        glDeleteShader(fragment_shader_id);
        return;
    }

    this->m_UUID = glCreateProgram();
    glAttachShader(this->m_UUID, vertex_shader_id);
    glAttachShader(this->m_UUID, fragment_shader_id);
    glLinkProgram(this->m_UUID);

    GLint success;
    glGetProgramiv(this->m_UUID, GL_LINK_STATUS, &success);
    if (success == GL_FALSE)
    {
        GLchar info_log[1024];
        glGetProgramInfoLog(this->m_UUID, 1024, nullptr, info_log);
        LCRITICAL("SHADER PROGRAM: Link-time error:\n{0}", info_log);
        this->finalize();
        glDeleteShader(vertex_shader_id);
        glDeleteShader(fragment_shader_id);
        return;
    }
    else this->m_is_compiled = true;

    glDetachShader(this->m_UUID, vertex_shader_id);
    glDetachShader(this->m_UUID, fragment_shader_id);
    glDeleteShader(vertex_shader_id);
    glDeleteShader(fragment_shader_id);
}

Shader::Shader(Shader&& shader) noexcept : m_UUID(shader.m_UUID), m_is_compiled(shader.m_is_compiled)
{
    shader.m_UUID = 0;
    shader.m_is_compiled = false;
}

Shader& Shader::operator=(Shader&& shader) noexcept
{
    this->finalize();
    this->m_UUID = shader.m_UUID;
    this->m_is_compiled = shader.m_is_compiled;

    shader.m_UUID = 0;
    shader.m_is_compiled = false;

    return *this;
}

void Shader::set_matrix4(const char* name, const glm::mat4& matrix) const
{
    glUniformMatrix4fv(glGetUniformLocation(this->m_UUID, name), 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::set_matrix3(const char* name, const glm::mat3& matrix) const
{
    glUniformMatrix3fv(glGetUniformLocation(this->m_UUID, name), 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::set_int(const char* name, const int value) const
{
    glUniform1i(glGetUniformLocation(this->m_UUID, name), value);
}

void Shader::set_float(const char* name, const float value) const
{
    glUniform1f(glGetUniformLocation(this->m_UUID, name), value);
}

void Shader::set_vec3(const char* name, const glm::vec3& value) const
{
    glUniform3f(glGetUniformLocation(this->m_UUID, name), value.x, value.y, value.z);
}

void Shader::set_vec4(const char* name, const glm::vec4& value) const
{
    glUniform4f(glGetUniformLocation(this->m_UUID, name), value.x, value.y, value.z, value.w);
}

void Shader::  bind() const { glUseProgram(this->m_UUID); }
void Shader::unbind()       { glUseProgram(0);            }

void Shader::finalize() { glDeleteProgram(this->m_UUID); }
     Shader::~Shader()  { this->finalize();              }