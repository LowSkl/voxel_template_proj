#pragma once

#include "SneakyThings.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
	unsigned int m_UUID = 0;
    bool m_is_compiled = false;

	void finalize();

public:
    static Shader load_shader(const char* vertexFile, const char* fragmentFile);
    static bool create_shader(const char* code, const ShaderType shaderType, unsigned int& shaderUUID);

    Shader(const char* vertexCode, const char* fragmentCode);
    Shader(Shader&& shader) noexcept;
    Shader& operator=(Shader&& shader) noexcept;
    ~Shader();

    Shader()                         = delete;
    Shader(const Shader&)            = delete;
    Shader& operator=(const Shader&) = delete;

    void bind() const;
    static void unbind();

    unsigned int get_UUID() const { return m_UUID;        }
    bool is_compiled()      const { return m_is_compiled; }

    void set_matrix4(const char* name, const glm::mat4& matrix) const;
    void set_matrix3(const char* name, const glm::mat3& matrix) const;
    void set_int(const char* name, const int value) const;
    void set_float(const char* name, const float value) const;
    void set_vec3(const char* name, const glm::vec3& value) const;
    void set_vec4(const char* name, const glm::vec4& value) const;
};

