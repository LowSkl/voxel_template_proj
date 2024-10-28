#include "Shader.h"

#include <fstream>
#include <sstream>

#include <utils/Log.h>
#include <utils/FileUtils.h>

#include <glm/gtc/type_ptr.hpp>

// ��������� ������ �� �����
Shader* Shader::load_shader(const std::string vertexFile, const std::string fragmentFile) {
	std::string vertexCode;
	std::string fragmentCode;

	try { // �������� ���, ��� �������� � ������
		vertexCode = get_fileContents(vertexFile);
		fragmentCode = get_fileContents(fragmentFile);
	} catch (std::ifstream::failure& e) {
		LCRITICAL("ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ");
		return nullptr;
	}

	return new Shader(vertexCode, fragmentCode);
}

Shader::Shader(const std::string vertexCode, const std::string fragmentCode) : m_UUID(-1) {
	const GLchar* vShaderCode = vertexCode.c_str();
	const GLchar* fShaderCode = fragmentCode.c_str();

	GLuint vertex, fragment;
	GLint success;
	GLchar infoLog[512];

	// Vertex Shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, nullptr);
	glCompileShader(vertex);
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertex, 512, nullptr, infoLog);
		LCRITICAL("SHADER::VERTEX: compilation failed");
		LCRITICAL(infoLog);
		return;
	}

	// Fragment Shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, nullptr);
	glCompileShader(fragment);
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragment, 512, nullptr, infoLog);
		LCRITICAL("SHADER::FRAGMENT: compilation failed");
		LCRITICAL(infoLog);
		return;
	}

	// Shader Program
	GLuint id = glCreateProgram();
	glAttachShader(id, vertex);
	glAttachShader(id, fragment);
	glLinkProgram(id);

	glGetProgramiv(id, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(id, 512, nullptr, infoLog);
		LCRITICAL("SHADER::PROGRAM: linking failed");
		LCRITICAL(infoLog);

		glDeleteShader(vertex);
		glDeleteShader(fragment);
		return;
	}

	this->m_UUID = id;

	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

// �������� ������� �������������
void Shader::uniformMatrix(const std::string name, const glm::mat4 matrix) {
	GLuint transformLoc = glGetUniformLocation(this->m_UUID, name.c_str());
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(matrix));
}

// ��������� ������
void Shader::bind() {
	glUseProgram(this->m_UUID);
}

// ��������� ������
void Shader::unbind() {
	glUseProgram(0);
}

// ������� ������
void Shader::finalize() {
	glDeleteProgram(this->m_UUID);
}

// ��������� � ������� ������
Shader::~Shader() { 
	this->unbind();
	this->finalize(); 
}