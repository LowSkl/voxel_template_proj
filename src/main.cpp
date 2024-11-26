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
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <math.h>

#include <modules/Camera.h>

int main()
{
    static float verts[]{
        //     COORDINATES      /        COLORS /   TexCoord    //
       -0.5f, -0.5f, -0.5f,     1.0f, 0.0f, 0.0f,   0.0f, 0.0f, // 0
       -0.5f,  0.5f, -0.5f,     0.0f, 1.0f, 0.0f,   0.0f, 1.0f, // 1
        0.5f,  0.5f, -0.5f,     0.0f, 0.0f, 1.0f,   1.0f, 1.0f, // 2
        0.5f, -0.5f, -0.5f,     1.0f, 1.0f, 1.0f,   1.0f, 0.0f, // 3
       -0.5f, -0.5f,  0.5f,     1.0f, 0.0f, 1.0f,   0.0f, 0.0f, // 4
       -0.5f,  0.5f,  0.5f,     0.0f, 1.0f, 1.0f,   0.0f, 1.0f, // 5
        0.5f,  0.5f,  0.5f,     1.0f, 1.0f, 0.0f,   1.0f, 1.0f, // 6
        0.5f, -0.5f,  0.5f,     1.0f, 1.0f, 1.0f,   1.0f, 0.0f, // 7
    };

    static const unsigned int indices[]{
        0, 1, 2, 0, 2, 3, // Back face
        4, 5, 6, 4, 6, 7, // Front face
        0, 4, 5, 0, 5, 1, // Left face
        3, 7, 6, 3, 6, 2, // Right face
        1, 5, 6, 1, 6, 2, // Top face
        0, 3, 7, 0, 7, 4  // Bottom face
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

    auto camera = Camera();
    camera.set_field_of_view(90);

    float lastTime = render->getTime();
    float delta = 0.0f;
    float speed = 1.f;

    while (!window.is_shouldClose())
    {
        float currentTime = render->getTime();
        delta = currentTime - lastTime;
        lastTime = currentTime;

        input->update();

        if (window.is_iconified()) continue;
        if (input->is_keyJustPressed(KeyCode::KEY_Q)) window.toggleCursor();

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

        glm::mat4 Model = glm::mat4(1.0f); {

            Model = glm::scale(glm::mat4(1.0f), glm::vec3(scale[0], scale[1], scale[2]));
            Model = glm::translate(Model, glm::vec3(trans[0], trans[1], trans[2]));

            Model = glm::rotate(Model, glm::radians(rotate[0]), glm::vec3(1, 0, 0));
            Model = glm::rotate(Model, glm::radians(rotate[1]), glm::vec3(0, 1, 0));
            Model = glm::rotate(Model, glm::radians(rotate[2]), glm::vec3(0, 0, 1));
        }

        if (window.is_cursorLocked()) camera.rotate(input->get_mousePosDeltaY() / window.get_height() * 2, input->get_mousePosDeltaX() / window.get_height() * 2, 0.f);

        if (input->is_keyPressed(KeyCode::KEY_W)) camera.move_front(delta * speed);
        if (input->is_keyPressed(KeyCode::KEY_A)) camera.move_right(delta * -speed);
        if (input->is_keyPressed(KeyCode::KEY_S)) camera.move_front(delta * -speed);
        if (input->is_keyPressed(KeyCode::KEY_D)) camera.move_right(delta * speed);

        if (input->is_keyPressed(KeyCode::KEY_LEFT_SHIFT)) camera.move_up(delta * -speed);
        if (input->is_keyPressed(KeyCode::KEY_SPACE))      camera.move_up(delta * speed);

        shader->set_matrix4("MVP", camera.get_projection_matrix() * camera.get_view_matrix() * Model);
        render->draw(*vao);

        //UI
        {
            ui->updateBegin();
            glm::vec2 win_pos = window.get_pos();

            ImGui::GetBackgroundDrawList()->AddText({ win_pos.x, win_pos.y }, ImGui::GetColorU32({ 0, 0, 0, 255 }), std::to_string(input->get_currentFrame()).c_str());

            ImGui::SetNextWindowContentSize({ 500, 200 });
            ImGui::Begin("world settings");
            {
                ImGui::SliderFloat3("scale", scale, -2.5, 2.5);
                ImGui::SliderFloat3("rotate", rotate, 0, 360);
                ImGui::SliderFloat3("trans", trans, -2.5, 2.5);
                ImGui::ColorEdit4("background", color);
            }
            ImGui::End();
            ui->updateEnd();
        }

        window.update();
    }
}
