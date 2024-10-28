#include "EBO.h"

// ������� ������ � ���������� ID � �������� ������
EBO::EBO(const GLuint* const indices, const GLsizeiptr size, const GLenum usage, bool unbind) : m_UUID(-1) {
	// ���������� �����, ��������� ���, ���������� ������
	glGenBuffers(1, &this->m_UUID);
	this->bind();
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, usage);

	// ������� ��� ���������� ����, ����� �� ������ ������ ��� ebo->bind()
	if (unbind) this->unbind();
}

// ��������� EBO
void EBO::bind() {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->m_UUID);
}

// ��������� EBO
void EBO::unbind() {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

// �������� EBO
void EBO::finalize() {
	glDeleteBuffers(1, &this->m_UUID);
}

// �������� ������, �� ����� ������
void EBO::reload(const GLuint* const indices, const GLsizeiptr size, const GLenum usage, bool unbind) {
	// ��������� �����, ���������� ������
	this->bind();
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, usage);

	// ������� ��� ���������� ����, ����� �� ������ ������ ��� ebo->bind()
	if (unbind) this->unbind();
}

// ��������� � ������� �����
EBO::~EBO() {
	this->unbind();
	this->finalize();
}