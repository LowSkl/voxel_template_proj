#pragma once

#include <string>

#include<glad/glad.h>
#include <glm/glm.hpp>

class Shader {
	GLuint ID;

public:
	Shader(std::string vertexCode, std::string fragmentCode);
	Shader(GLuint id);

	virtual ~Shader();

	int initialize();
	void finalize();

	void uniformMatrix(std::string name, glm::mat4 matrix);
	static Shader* load_shader(std::string vertexFile, std::string fragmentFile);

	GLuint get_id() const { return this->ID; }
};

