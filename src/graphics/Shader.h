#pragma once

#include <string>

#include <glad/glad.h>
#include <glm/glm.hpp>

class Shader {
	GLuint m_UUID;

public:
	Shader(const std::string vertexCode, const std::string fragmentCode);
	virtual ~Shader();

	// ��������� ������
	void bind();

	// ��������� ������
	static void unbind();

	// ������� ������
	void finalize();

	// �������� ������� �������������
	void uniformMatrix(const std::string name, const glm::mat4 matrix);

	// ��������� ������ �� �����
	static Shader* load_shader(const std::string vertexFile, const std::string fragmentFile);

	GLuint get_UUID() const { return this->m_UUID; }
};

