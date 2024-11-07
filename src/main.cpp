#include <iostream>
#include <format>

#include <window/Window.h>
#include <window/Input.h>
#include <modules/UI.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui_internal.h>

#include <utils/Log.h>

#include <graphics/Shader.h>
#include <graphics/Texture.h>

#include <graphics/buffers/EBO.h>
#include <graphics/buffers/VBO.h>
#include <graphics/buffers/VAO.h>

#include <glm/glm.hpp>
#include <math.h>

int main()
{
    static float verts[]{
        //     COORDINATES     /        COLORS      /   TexCoord  //
           -0.5f, -0.5f, 0.0f,     1.0f, 0.0f, 0.0f,	0.0f, 0.0f, // Lower left corner 1
           -0.5f,  0.5f, 0.0f,     0.0f, 1.0f, 0.0f,	0.0f, 1.0f, // Upper left corner 1
            0.5f,  0.5f, 0.0f,     0.0f, 0.0f, 1.0f,	1.0f, 1.0f, // Upper right corner 1
            0.5f, -0.5f, 0.0f,     1.0f, 1.0f, 1.0f,	1.0f, 0.0f,  // Lower right corner 1
    };

    static const unsigned int indices[]{
        0, 2, 1, // Upper triangle 1
        0, 3, 2, // Lower triangle 1
    };

    BufferLayout types {
        ShaderDataType::Float3,
        ShaderDataType::Float3,
        ShaderDataType::Float2
    };

    Window window("render test", 800, 800);

    RenderOpenGL* render = window.get_render();
    Input*        input  = window.get_input();
    UI*           ui     = window.get_ui();

    render->enable_depth_test();

    static const unsigned short texture_slot = 0;

    auto shader  = std::shared_ptr<Shader>(Shader::load_shader("resources/shaders/default.vert", "resources/shaders/default.frag"));
    auto texture = std::shared_ptr<Texture>(Texture::load_texture("resources/textures/pop_cat.png", TextureType::Texture_2D, texture_slot, TextureFormat::RGBA, PixelType::Unsigned_byte));

    float color [4]{ 1, 1, 1, 1 };

    float scale[3]{ 1, 1, 1 };
    float rotate[3]{ 0, 0, 0 };
    float trans[3]{ 0, 0, 0 };

    float* verts_col[4]{ &verts[3], &verts[11], &verts[19], &verts[27] };

    while (!window.is_shouldClose())
    {
        render->clear();
        render->set_clear_color(color[0], color[1], color[2], color[3]);

        shader->bind();

        auto vao = std::make_shared<VAO>();
        auto vbo = std::make_shared<VBO>(verts, sizeof(verts), types);
        auto ebo = std::make_shared<EBO>(indices, sizeof(indices));

        vao->add_vertexBuffer(*vbo);
        vao->set_indexBuffer(*ebo);

        vao->bind();

        shader->set_int("tex_slot", texture_slot);

        glm::mat4 Model = glm::mat4(1.0f);

        Model = glm::scale(glm::mat4(1.0f), glm::vec3(scale[0], scale[1], scale[2]));
        Model = glm::translate(Model, glm::vec3(trans[0], trans[1], trans[2]));

        Model = glm::rotate(Model, glm::radians(rotate[0]), glm::vec3(1, 0, 0));
        Model = glm::rotate(Model, glm::radians(rotate[1]), glm::vec3(0, 1, 0));
        Model = glm::rotate(Model, glm::radians(rotate[2]), glm::vec3(0, 0, 1));

        glm::mat4 MVP = Model;

        shader->set_matrix4("MVP_matrix", MVP);
        render->draw(*vao);

        //UI
        {
            ui->updateBegin();

            ImGui::SetNextWindowContentSize({ 500, 200 });
            ImGui::Begin("world settings");
            {
                ImGui::SliderFloat3("scale", scale, -2.5, 2.5);
                ImGui::SliderFloat3("rotate", rotate, 0, 360);
                ImGui::SliderFloat3("trans", trans, -2.5, 2.5);
                ImGui::ColorEdit4("background", color);
            }
            ImGui::End();
            
            ImGui::SetNextWindowContentSize({ 500, 200 });
            ImGui::Begin("verts settings");
            {
                ImGui::ColorEdit3("vert 0 color", verts_col[0]);
                ImGui::ColorEdit3("vert 1 color", verts_col[1]);
                ImGui::ColorEdit3("vert 2 color", verts_col[2]);
                ImGui::ColorEdit3("vert 3 color", verts_col[3]);
            }
            ImGui::End();
            ui->updateEnd();
        }

        window.update();
        input->update();
    }
}
