#include "VAO.h"

// ������� ������ � ���������� ID
VAO::VAO(bool bind) : m_UUID(-1) {
	glGenVertexArrays(1, &this->m_UUID);

	// ������� ��� ���������� ����, ����� �� ������ ������ ��� vao->bind()
	if (bind) this->bind();
}

// ������������ �������
void VAO::linkAttrib(VBO& const VBO, const GLuint layout, const GLuint numComponents, const GLenum type, const GLsizeiptr stride, const void* const offset, bool unbind) {
	VBO.bind();
	glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset);
	glEnableVertexAttribArray(layout);
	
	// ������� ��� ���������� ����, ����� �� ������ ������ ��� vao->bind()
	if (unbind) VBO.unbind();
}

// ��������� VAO
void VAO::bind() {
	glBindVertexArray(this->m_UUID);
}

// ��������� VAO
void VAO::unbind() {
	glBindVertexArray(0);
}

// �������� VAO
void VAO::finalize() {
	glDeleteVertexArrays(1, &this->m_UUID);
}

// ��������� � ������� �����
VAO::~VAO() {
	this->unbind();
	this->finalize();
}