#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex>* vertices, std::vector<unsigned int>* indices, std::vector<Texture>* textures) :
	m_localSpace(vertices),
	m_vbo(vertices->data(), vertices->size() * sizeof(Vertex), this->m_dataLayout),
	m_ebo(indices->data(), indices->size() * sizeof(unsigned int)),
	m_textures(textures)
{
	this->m_vao.add_vertexBuffer(this->m_vbo);
	this->m_vao.set_indexBuffer(this->m_ebo);

	this->m_vao.unbind();
	this->m_vbo.unbind();
	this->m_ebo.unbind();
}

void Mesh::draw(Shader* shader, CameraControl* camera, Primitivs primitiv, RenderOpenGL* renderer)
{
	shader->bind();
	this->m_vao.bind();

	unsigned int numDiffuseTex  = 0;
	unsigned int numSpecularTex = 0;

	if (this->m_textures != nullptr)
	for (int i = 0; i < this->m_textures->size(); ++i)
	{
		Texture* texture = &(*this->m_textures)[i];

		std::string num;
		std::string type = texture_type_to_string(texture->get_textureType());

		switch (texture->get_textureType())
		{
		case TextureType::DIFFUSE:  num = std::to_string(numDiffuseTex++);  break;
		case TextureType::SPECULAR: num = std::to_string(numSpecularTex++); break;
		}

		texture->bind();
		shader->set_int((type + num).c_str(), i);
	}
	shader->set_matrix4("camMatrix", camera->get_matricies());
	shader->set_vec3("camPos", camera->get_camera()->get_position());

	renderer->draw(this->m_vao, primitiv);
}