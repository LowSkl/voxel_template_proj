#include "VBO.h"

// ������� ������ � ���������� ID � �������� ������
VBO::VBO(const GLfloat* vertices, const GLsizeiptr size, const GLenum usage, bool unbind) : m_UUID(-1) {
	// ���������� �����, ��������� ���, ���������� ������
	glGenBuffers(1, &this->m_UUID);
	this->bind();
	glBufferData(GL_ARRAY_BUFFER, size, vertices, usage);

	// ������� ��� ���������� ����, ����� �� ������ ������ ��� vbo->bind()
	if (unbind) this->unbind();
}

// ��������� VBO
void VBO::bind() {
	glBindBuffer(GL_ARRAY_BUFFER, this->m_UUID);
}

// ��������� VBO
void VBO::unbind() {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

// �������� VBO
void VBO::finalize() {
	glDeleteBuffers(1, &this->m_UUID);
}

// �������� ������, �� ����� ������
void VBO::reload(const GLfloat* vertices, const GLsizeiptr size, const GLenum usage, bool unbind) {
	// ��������� �����, ���������� ������
	this->bind();
	glBufferData(GL_ARRAY_BUFFER, size, vertices, usage);

	// ������� ��� ���������� ����, ����� �� ������ ������ ��� vbo->bind()
	if (unbind) this->unbind();
}

// ��������� � ������� �����
VBO::~VBO() {
	this->unbind();
	this->finalize();
}