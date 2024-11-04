#include <iostream>
#include <format>

#include <window/Window.h>
#include <window/Input.h>
#include <modules/UI.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui_internal.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <utils/Log.h>

#include <graphics/Shader.h>
#include <graphics/Texture.h>

#include <graphics/buffers/EBO.h>
#include <graphics/buffers/VBO.h>
#include <graphics/buffers/VAO.h>

int main()
{
    std::shared_ptr<Window> windows[4];

    std::shared_ptr<Shader> shaders[4];
    std::shared_ptr<Texture> textures[4];

    std::shared_ptr<VAO> vaos[4];
    std::shared_ptr<VBO> vbos[4];
    std::shared_ptr<EBO> ebos[4];

    BufferLayout lay_down_NOW_23_yo
    {
        ShaderDataType::Float3,
        ShaderDataType::Float3,
        ShaderDataType::Float2
    };

    static const GLfloat verts[]{
        //     COORDINATES     /        COLORS      /   TexCoord  //
           -0.5f, -0.5f, 0.0f,     1.0f, 0.0f, 0.0f,	0.0f, 0.0f, // Lower left corner 1
           -0.5f,  0.5f, 0.0f,     0.0f, 1.0f, 0.0f,	0.0f, 1.0f, // Upper left corner 1
            0.5f,  0.5f, 0.0f,     0.0f, 0.0f, 1.0f,	1.0f, 1.0f, // Upper right corner 1
            0.5f, -0.5f, 0.0f,     1.0f, 1.0f, 1.0f,	1.0f, 0.0f,  // Lower right corner 1
    };

    static const GLuint indices[]{
        0, 2, 1, // Upper triangle 1
        0, 3, 2, // Lower triangle 1
    };

    const unsigned short unit = 0;

	for (size_t i = 0; i < 4; i++)
	{
        const struct
        {
            float r, g, b;
        } colors[] =
        {
            { 0.95f, 0.32f, 0.11f },
            { 0.50f, 0.80f, 0.16f },
            {   0.f, 0.68f, 0.94f },
            { 0.98f, 0.74f, 0.04f }
        };

		windows[i] = std::make_shared<Window>(std::format("test {}", i+1), 500, 500);
        windows[i]->get_render()->enable_depth_test();
        glClearColor(colors[i].r, colors[i].g, colors[i].b, 1.f);

        //test zone
        shaders[i] = std::shared_ptr<Shader>(Shader::load_shader("resources/shaders/default.vert", "resources/shaders/default.frag"));
        if (!shaders[i]->is_compiled()) { LCRITICAL("uhh..."); return 1; }

        textures[i] = std::shared_ptr<Texture>(Texture::load_texture("resources/textures/pop_cat.png", TextureType::Texture_2D, unit, TextureFormat::RGBA, PixelType::Unsigned_byte));

        vaos[i] = std::make_shared<VAO>();
        vbos[i] = std::make_shared<VBO>(verts, sizeof(verts), lay_down_NOW_23_yo);
        ebos[i] = std::make_shared<EBO>(indices, sizeof(indices) / sizeof(GLuint));

        vaos[i]->add_vertexBuffer(*vbos[i]);
        vaos[i]->set_indexBuffer(*ebos[i]);
        //test zone

    }

    while (Window::get_windowsCount() != 0)
    {
        for (int i = 0; i < 4; i++)
        {
            if (windows[i] == nullptr)
                continue;

            windows[i]->get_ui()->updateBegin();

            ImGui::Begin(windows[i]->get_title()->c_str());
            ImGui::Text("hi %d", windows[i]->get_ui()->get_count());
            ImGui::End();

            windows[i]->get_render()->clear();
            windows[i]->get_ui()->updateEnd();

            //test zone
            shaders[i]->bind();
            vaos[i]->bind();

            shaders[i]->set_int("tex0", unit);

            windows[i]->get_render()->draw(*vaos[i]); 
            //test zone

            windows[i]->update();
            Input::update();

            if (windows[i]->is_shouldClose())
                windows[i] = nullptr;
        }
    }
}
