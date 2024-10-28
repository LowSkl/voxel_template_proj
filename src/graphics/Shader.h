#pragma once

#include <string>

#include <glad/glad.h>
#include <glm/glm.hpp>

class Shader {
	GLuint m_UUID;

public:
	Shader(const std::string vertexCode, const std::string fragmentCode);
	virtual ~Shader();

	// Загрузить шейдер
	void bind();

	// Выгрузить шейдер
	static void unbind();

	// Удалить шейдер
	void finalize();

	// Вставить матрицу трансформации
	void uniformMatrix(const std::string name, const glm::mat4 matrix);

	// Загрузить шейдер из файла
	static Shader* load_shader(const std::string vertexFile, const std::string fragmentFile);

	GLuint get_UUID() const { return this->m_UUID; }
};

