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

#include <modules/camera/CameraControl.h>

int main()
{
    // Vertices coordinates
    static const float pyramide_vertices[] =
    { //     COORDINATES     /        COLORS          /    TexCoord   /        NORMALS       //
	    -0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f, 	 0.0f, 0.0f,      0.0f, -1.0f, 0.0f, // Bottom side
	    -0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 0.0f, 5.0f,      0.0f, -1.0f, 0.0f, // Bottom side
	     0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 5.0f,      0.0f, -1.0f, 0.0f, // Bottom side
	     0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 0.0f,      0.0f, -1.0f, 0.0f, // Bottom side

	    -0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f, 	 0.0f, 0.0f,     -0.8f, 0.5f,  0.0f, // Left Side
	    -0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 0.0f,     -0.8f, 0.5f,  0.0f, // Left Side
	     0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	 2.5f, 5.0f,     -0.8f, 0.5f,  0.0f, // Left Side

	    -0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 0.0f,      0.0f, 0.5f, -0.8f, // Non-facing side
	     0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 0.0f, 0.0f,      0.0f, 0.5f, -0.8f, // Non-facing side
	     0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	 2.5f, 5.0f,      0.0f, 0.5f, -0.8f, // Non-facing side

	     0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 0.0f, 0.0f,      0.8f, 0.5f,  0.0f, // Right side
	     0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 0.0f,      0.8f, 0.5f,  0.0f, // Right side
	     0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	 2.5f, 5.0f,      0.8f, 0.5f,  0.0f, // Right side

	     0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 0.0f,      0.0f, 0.5f,  0.8f, // Facing side
	    -0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f, 	 0.0f, 0.0f,      0.0f, 0.5f,  0.8f, // Facing side
	     0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	 2.5f, 5.0f,      0.0f, 0.5f,  0.8f  // Facing side
    };

    // Indices for vertices order
    static const unsigned int pyramide_indices[] =
    {
        0, 1, 2, // Bottom side
        0, 2, 3, // Bottom side
        4, 6, 5, // Left side
        7, 9, 8, // Non-facing side
        10, 12, 11, // Right side
        13, 15, 14 // Facing side
    };

    GLfloat light_vertices[] =
    { //     COORDINATES     //
        -0.1f, -0.1f,  0.1f,
        -0.1f, -0.1f, -0.1f,
         0.1f, -0.1f, -0.1f,
         0.1f, -0.1f,  0.1f,
        -0.1f,  0.1f,  0.1f,
        -0.1f,  0.1f, -0.1f,
         0.1f,  0.1f, -0.1f,
         0.1f,  0.1f,  0.1f
    };

    GLuint light_indices[] =
    {
        0, 1, 2,
        0, 2, 3,
        0, 4, 7,
        0, 7, 3,
        3, 7, 6,
        3, 6, 2,
        2, 6, 5,
        2, 5, 1,
        1, 5, 4,
        1, 4, 0,
        4, 5, 6,
        4, 6, 7
    };


    BufferLayout pyramide_types {
        ShaderDataType::Float3,
        ShaderDataType::Float3,
        ShaderDataType::Float2,
        ShaderDataType::Float3,
    };

    BufferLayout light_types{
        ShaderDataType::Float3,
    };

    Window window("render test", 800, 800);

    RenderOpenGL* render = window.get_render();
    Input*        input  = window.get_input();
    UI*           ui     = window.get_ui();

    render->enable_depth_test();

    static const unsigned short texture_slot = 0;

    auto pyramide_shader = std::shared_ptr<Shader>(Shader::load_shader("resources/shaders/default.vert", "resources/shaders/default.frag"));
    auto light_shader    = std::shared_ptr<Shader>(Shader::load_shader("resources/shaders/light.vert",   "resources/shaders/light.frag"  ));

    auto texture = std::shared_ptr<Texture>(Texture::load_texture("resources/textures/brick.png", TextureType::Texture_2D, texture_slot, TextureFormat::RGBA, PixelType::Unsigned_byte));

    float back_color [4]{ 1, 60, 60, 1 };

    float pyr_scale[3]{ 1, 1, 1 };
    float pyr_rotate[3]{ 0, 0, 0 };
    float pyr_trans[3]{ 0, 0, 0 };

    float light_rotate[3]{ 0, 0, 0 };
    float light_trans[3]{ 0, 0, 2 };

    Camera camera = Camera();
    camera.set_field_of_view(90);

    CameraControl cameraControl = CameraControl(&window, &camera);

    while (!window.is_shouldClose())
    {
        input->update();

        if (window.is_iconified()) continue;
        if (input->is_keyJustPressed(KeyCode::KEY_Q)) window.toggleCursor();
        cameraControl.update();

        render->set_clear_color(back_color[0], back_color[1], back_color[2], back_color[3]);
        render->clear();

        /* LIGHT */
        light_shader->bind();

        auto light_vao = std::make_shared<VAO>();
        auto light_vbo = std::make_shared<VBO>(light_vertices, sizeof(light_vertices), light_types);
        auto light_ebo = std::make_shared<EBO>(light_indices, sizeof(light_indices));

        light_vao->add_vertexBuffer(*light_vbo);
        light_vao->set_indexBuffer(*light_ebo);

        light_vao->bind();

        glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
        glm::mat4 light_model = glm::mat4(1.0f); {
            light_model = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 0.5f));
            light_model = glm::translate(light_model, glm::vec3(light_trans[0], light_trans[1], light_trans[2]));

            light_model = glm::rotate(light_model, glm::radians(light_rotate[0]), glm::vec3(1, 0, 0));
            light_model = glm::rotate(light_model, glm::radians(light_rotate[1]), glm::vec3(0, 1, 0));
            light_model = glm::rotate(light_model, glm::radians(light_rotate[2]), glm::vec3(0, 0, 1));
        }
        light_shader->set_matrix4("model", light_model);
        light_shader->set_matrix4("camMatrix", cameraControl.get_matricies());
        light_shader->set_vec4("lightColor", lightColor);
        render->draw(*light_vao, Primitivs::TRIANGLES);

        /* PYRAMIDE */
        pyramide_shader->bind();

        auto pyramide_vao = std::make_shared<VAO>();
        auto pyramide_vbo = std::make_shared<VBO>(pyramide_vertices, sizeof(pyramide_vertices), pyramide_types);
        auto pyramide_ebo = std::make_shared<EBO>(pyramide_indices, sizeof(pyramide_indices));

        pyramide_vao->add_vertexBuffer(*pyramide_vbo);
        pyramide_vao->set_indexBuffer(*pyramide_ebo);

        pyramide_vao->bind();
        pyramide_shader->set_int("tex_slot", texture_slot);

        glm::mat4 pyramide_model = glm::mat4(1.0f); {

            pyramide_model = glm::scale(glm::mat4(1.0f), glm::vec3(pyr_scale[0], pyr_scale[1], pyr_scale[2]));
            pyramide_model = glm::translate(pyramide_model, glm::vec3(pyr_trans[0], pyr_trans[1], pyr_trans[2]));

            pyramide_model = glm::rotate(pyramide_model, glm::radians(pyr_rotate[0]), glm::vec3(1, 0, 0));
            pyramide_model = glm::rotate(pyramide_model, glm::radians(pyr_rotate[1]), glm::vec3(0, 1, 0));
            pyramide_model = glm::rotate(pyramide_model, glm::radians(pyr_rotate[2]), glm::vec3(0, 0, 1));
        }

        pyramide_shader->set_matrix4("model", pyramide_model);
        pyramide_shader->set_matrix4("camMatrix", cameraControl.get_matricies());

        pyramide_shader->set_vec4("lightColor", lightColor);
        pyramide_shader->set_vec3("lightPos", glm::vec3(light_trans[0], light_trans[1], light_trans[2]));
        pyramide_shader->set_vec3("camPos", camera.get_position());

        render->draw(*pyramide_vao, Primitivs::TRIANGLES);

        if (!window.is_cursorLocked())
        {
            ui->updateBegin();
            glm::vec2 win_pos = window.get_pos();

            ImGui::GetBackgroundDrawList()->AddText({ win_pos.x, win_pos.y }, ImGui::GetColorU32({ 0, 0, 0, 255 }), std::to_string(input->get_currentFrame()).c_str());

            ImGui::SetNextWindowContentSize({ 500, 200 });
            ImGui::SetNextWindowPos({ win_pos.x, win_pos.y });
            ImGui::Begin("world settings");
            {
                ImGui::ColorEdit4("background", back_color);
            } ImGui::End();

            ImGui::SetNextWindowContentSize({ 500, 200 });
            ImGui::SetNextWindowPos({ win_pos.x, win_pos.y + 300 });
            ImGui::Begin("pyramide settings");
            {
                ImGui::SliderFloat3("scale", pyr_scale, -2.5, 2.5);
                ImGui::SliderFloat3("rotate", pyr_rotate, 0, 360);
                ImGui::SliderFloat3("trans", pyr_trans, -2.5, 2.5);
            } ImGui::End();

            ImGui::SetNextWindowContentSize({ 500, 200 });
            ImGui::SetNextWindowPos({ win_pos.x, win_pos.y + 600 });
            ImGui::Begin("light settings");
            {
                ImGui::SliderFloat3("rotate", light_rotate, 0, 360);
                ImGui::SliderFloat3("trans", light_trans, -2.5, 2.5);
            } ImGui::End();

            ui->updateEnd();
        }

        window.update();
    }
}
