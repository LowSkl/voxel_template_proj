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
#include <objects/Mesh.h>

int main()
{
    Window window("render test", 800, 800);

    RenderOpenGL* render = window.get_render();
    Input*        input  = window.get_input();
    UI*           ui     = window.get_ui();

    render->enable_depth_test();

    auto floor_shader = Shader::load_shader("resources/shaders/default.vert", "resources/shaders/default.frag");
    auto light_shader = Shader::load_shader("resources/shaders/light.vert",   "resources/shaders/light.frag"  );

    float back_color [4]{ 0.07f, 0.13f, 0.17f, 1.0f };

    glm::vec3 floorPos = glm::vec3(0.5f, 0.5f, 0.5f);
    glm::vec3 floorRotate = glm::vec3(0.5f, 0.5f, 0.5f);

    glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);

    Camera camera = Camera();
    camera.set_field_of_view(90);

    CameraControl cameraControl = CameraControl(&window, &camera);

    Vertex vertices[] =
    { //               COORDINATES           /            COLORS          /           NORMALS         /       TEXTURE COORDINATES    //
        Vertex{glm::vec3(-1.0f, 0.0f,  1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f)},
        Vertex{glm::vec3(-1.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f)},
        Vertex{glm::vec3(1.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f)},
        Vertex{glm::vec3(1.0f, 0.0f,  1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f)}
    };

    unsigned int indices[] =
    {
        0, 1, 2,
        0, 2, 3
    };

    Texture textures[]
    {
        Texture::load_texture("resources/textures/planks.png",     TextureTypeGL::Texture_2D, TextureType::DIFFUSE,  0, TextureFormat::RGBA, PixelType::Unsigned_byte),
        Texture::load_texture("resources/textures/planksSpec.png", TextureTypeGL::Texture_2D, TextureType::SPECULAR, 1, TextureFormat::RED,  PixelType::Unsigned_byte)   
    };

    std::vector <Vertex> verts(vertices, vertices + sizeof(vertices) / sizeof(Vertex));
    std::vector <unsigned int> ind(indices, indices + sizeof(indices) / sizeof(unsigned int));
    std::vector <Texture> tex(textures, textures + sizeof(textures) / sizeof(Texture));

    Mesh floor(&verts, &ind, &tex);

    Vertex lightVertices[] =
    { //     COORDINATES     //
        Vertex{glm::vec3(-0.1f, -0.1f,  0.1f)},
        Vertex{glm::vec3(-0.1f, -0.1f, -0.1f)},
        Vertex{glm::vec3(0.1f, -0.1f, -0.1f)},
        Vertex{glm::vec3(0.1f, -0.1f,  0.1f)},
        Vertex{glm::vec3(-0.1f,  0.1f,  0.1f)},
        Vertex{glm::vec3(-0.1f,  0.1f, -0.1f)},
        Vertex{glm::vec3(0.1f,  0.1f, -0.1f)},
        Vertex{glm::vec3(0.1f,  0.1f,  0.1f)}
    };

    unsigned int lightIndices[] =
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

    std::vector <Vertex> lightVerts(lightVertices, lightVertices + sizeof(lightVertices) / sizeof(Vertex));
    std::vector <unsigned int> lightInd(lightIndices, lightIndices + sizeof(lightIndices) / sizeof(unsigned int));

    Mesh light(&lightVerts, &lightInd);

    while (!window.is_shouldClose())
    {
        input->update();

        if (window.is_iconified()) continue;
        if (input->is_keyJustPressed(KeyCode::KEY_Q)) window.toggleCursor();
        cameraControl.update();

        render->set_clear_color(back_color[0], back_color[1], back_color[2], back_color[3]);
        render->clear();

        /* LIGHT */
        light_shader.bind();

        glm::mat4 light_model = glm::mat4(1.0f); {
            light_model = glm::translate(light_model, lightPos);
        }
        light_shader.set_matrix4("model", light_model);
        light_shader.set_vec4("lightColor", lightColor);

        light.draw(&light_shader, &cameraControl, Primitivs::TRIANGLES, render);

        /* FLOOR */
        floor_shader.bind();

        glm::mat4 pyramide_model = glm::mat4(1.0f); {
            pyramide_model = glm::translate(pyramide_model, floorPos);

            pyramide_model = glm::rotate(pyramide_model, glm::radians(floorRotate.x), glm::vec3(1, 0, 0));
            pyramide_model = glm::rotate(pyramide_model, glm::radians(floorRotate.y), glm::vec3(0, 1, 0));
            pyramide_model = glm::rotate(pyramide_model, glm::radians(floorRotate.z), glm::vec3(0, 0, 1));
        }

        floor_shader.set_matrix4("model", pyramide_model);
        floor_shader.set_vec4("lightColor", lightColor);
        floor_shader.set_vec3("lightPos", lightPos);

        floor.draw(&floor_shader, &cameraControl, Primitivs::TRIANGLES, render);

        if (!window.is_cursorLocked())
        {
            ui->updateBegin();
            glm::vec2 win_pos = window.get_pos();

            ImGui::GetBackgroundDrawList()->AddText({ win_pos.x, win_pos.y }, ImGui::GetColorU32({ 0, 0, 0, 255 }), std::to_string(input->get_currentFrame()).c_str());

            ImGui::Begin("world settings");
            {
                ImGui::ColorEdit4("background", back_color);
            } ImGui::End();

            ImGui::Begin("floor settings");
            {
                float* rot[]{ &floorRotate[0], &floorRotate[1], &floorRotate[2] };
                ImGui::SliderFloat3("rotate", rot[0], 0, 360);

                float* trans[]{ &floorPos[0], &floorPos[1], &floorPos[2] };
                ImGui::SliderFloat3("trans", trans[0], -2.5, 2.5);
            } ImGui::End();

            ImGui::Begin("light settings");
            {
                float* trans[]{ &lightPos[0], &lightPos[1], &lightPos[2] };
                ImGui::SliderFloat3("trans", trans[0], -2.5, 2.5);

                float* col[]{ &lightColor[0], &lightColor[1], &lightColor[2], &lightColor[3] };
                ImGui::ColorEdit4("col", col[0]);
            } ImGui::End();

            ui->updateEnd();
        }

        window.update();
    }
}
