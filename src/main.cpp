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

int main()
{
    static const float verts[]{
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

    Window window("render test", 500, 500);

    RenderOpenGL* render = window.get_render();
    Input*        input  = window.get_input();
    UI*           ui     = window.get_ui();

    render->enable_depth_test();
    render->set_clear_color(0.95f, 0.32f, 0.11f, 1.f);

    static const unsigned short texture_slot = 0;

    auto shader  = std::shared_ptr<Shader>(Shader::load_shader("resources/shaders/default.vert", "resources/shaders/default.frag"));
    auto texture = std::shared_ptr<Texture>(Texture::load_texture("resources/textures/pop_cat.png", TextureType::Texture_2D, texture_slot, TextureFormat::RGBA, PixelType::Unsigned_byte));

    auto vao = std::make_shared<VAO>();
    auto vbo = std::make_shared<VBO>(verts, sizeof(verts), types);
    auto ebo = std::make_shared<EBO>(indices, sizeof(indices));

    vao->add_vertexBuffer(*vbo);
    vao->set_indexBuffer(*ebo);

    while (!window.is_shouldClose())
    {
        render->clear();

        //UI
        {
            ui->updateBegin();
            ImGui::Begin("world settings");

            ImGui::End();
            ui->updateEnd();
        }

        shader->bind();
        vao->bind();

        shader->set_int("tex0", texture_slot);
        render->draw(*vao);

        window.update();
        input->update();
    }
}
