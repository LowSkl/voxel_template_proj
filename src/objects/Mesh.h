#pragma once

#include <graphics/buffers/VAO.h>
#include <graphics/SneakyThings.h>

#include <graphics/Shader.h>
#include <graphics/Texture.h>

#include <modules/camera/CameraControl.h>

#include <vector>

#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec3 color;
	glm::vec2 textureUV;
};

class Mesh
{
	std::vector<Vertex>*  m_localSpace;
	std::vector<Texture>* m_textures;

	BufferLayout m_dataLayout{
		ShaderDataType::Float3, // vertex position
		ShaderDataType::Float3, // vertex normal
		ShaderDataType::Float3, // vertex color
		ShaderDataType::Float2, // texture UV on vertex
	};

	VAO m_vao;
	VBO m_vbo;
	EBO m_ebo;

public:

	Mesh(std::vector<Vertex>* vertices, std::vector<unsigned int>* indices, std::vector<Texture>* textures = nullptr);

	Mesh(const Mesh&      ) = delete;
	Mesh(      Mesh&& mesh) = delete;

	Mesh& operator=(const Mesh&      ) = delete;
	Mesh& operator=(      Mesh&& mesh) = delete;

	void draw(Shader* shader, CameraControl* camera, Primitivs primitiv, RenderOpenGL* renderer);
};

