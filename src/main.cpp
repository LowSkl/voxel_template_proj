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

    static const unsigned short texture_slot = 0;

    auto shader  = std::shared_ptr<Shader>(Shader::load_shader("resources/shaders/default.vert", "resources/shaders/default.frag"));
    auto texture = std::shared_ptr<Texture>(Texture::load_texture("resources/textures/pop_cat.png", TextureType::Texture_2D, texture_slot, TextureFormat::RGBA, PixelType::Unsigned_byte));

    auto vao = std::make_shared<VAO>();
    auto vbo = std::make_shared<VBO>(verts, sizeof(verts), types);
    auto ebo = std::make_shared<EBO>(indices, sizeof(indices));

    vao->add_vertexBuffer(*vbo);
    vao->set_indexBuffer(*ebo);

    float scale [4]{ 1, 1, 1, 1 };
    float color [4]{ 1, 1, 1, 1 };

    float rotate[3]{ 0, 0, 0 };
    float transp[3]{ 0, 0, 0 };

    while (!window.is_shouldClose())
    {
        render->clear();
        render->set_clear_color(color[0], color[1], color[2], color[3]);

        shader->bind();
        vao->bind();

        shader->set_int("tex_slot", texture_slot);
        shader->set_matrix4("scale_mat", glm::mat4( scale[0], 0,        0,        0, 
                                                    0,        scale[1], 0,        0,
                                                    0,        0,        scale[2], 0,
                                                    0,        0,        0,        scale[3] ));


        float x_rot_rad = glm::radians(rotate[0]);
        float y_rot_rad = glm::radians(rotate[1]);
        float z_rot_rad = glm::radians(rotate[2]);

        glm::mat4 x_mat( cos(x_rot_rad), sin(x_rot_rad), 0,              0, 
                        -sin(x_rot_rad), cos(x_rot_rad), 0,              0,
                         0,              0,              1,              0,
                         0,              0,              0,              1 );

        glm::mat4 y_mat( cos(y_rot_rad), 0,              sin(y_rot_rad), 0,
                         0,              1,              0,              0,
                        -sin(y_rot_rad), 0,              cos(y_rot_rad), 0,
                         0,              0,              0,              1 );

        glm::mat4 z_mat( cos(z_rot_rad),-sin(z_rot_rad), 0,              0,
                         sin(z_rot_rad), cos(z_rot_rad), 0,              0,
                         0,              0,              1,              0,
                         0,              0,              0,              1 );

        shader->set_matrix4("rotate_mat", x_mat * y_mat * z_mat);
        shader->set_matrix4("transp_mat", glm::mat4( 1,         0,         0,         0,
                                                     0,         1,         0,         0,
                                                     0,         0,         1,         0,
                                                     transp[0], transp[1], transp[2], 1 ));

        render->draw(*vao);

        //UI
        {
            ui->updateBegin();
            ImGui::Begin("world settings");

            ImGui::SliderFloat4("scale", scale, -2.5, 2.5);
            ImGui::SliderFloat3("rotate", rotate, 0, 360);
            ImGui::SliderFloat3("transp", transp, -2.5, 2.5);
            ImGui::ColorEdit4("background", color);

            ImGui::End();
            ui->updateEnd();
        }

        window.update();
        input->update();
    }
}
