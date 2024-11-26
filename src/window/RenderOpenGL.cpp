#include <window/Window.h>
#include "RenderOpenGL.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <utils/Log.h>

unsigned short RenderOpenGL::m_renderCount = 0;

const char* gl_source_to_string(const GLenum source)
{
    switch (source)
    {
    case GL_DEBUG_SOURCE_API: return "DEBUG_SOURCE_API";
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM: return "DEBUG_SOURCE_WINDOW_SYSTEM";
    case GL_DEBUG_SOURCE_SHADER_COMPILER: return "GL_DEBUG_SOURCE_SHADER_COMPILER";
    case GL_DEBUG_SOURCE_THIRD_PARTY: return "DEBUG_SOURCE_THIRD_PARTY";
    case GL_DEBUG_SOURCE_APPLICATION: return "DEBUG_SOURCE_APPLICATION";
    case GL_DEBUG_SOURCE_OTHER: return "DEBUG_SOURCE_OTHER";

    default: return "UNKNOWN_DEBUG_SOURCE";
    }
}

const char* gl_type_to_string(const GLenum type)
{
    switch (type)
    {
    case GL_DEBUG_TYPE_ERROR: return "DEBUG_TYPE_ERROR";
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: return "DEBUG_TYPE_DEPRECATED_BEHAVIOR";
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: return "DEBUG_TYPE_UNDEFINED_BEHAVIOR";
    case GL_DEBUG_TYPE_PORTABILITY: return "DEBUG_TYPE_PORTABILITY";
    case GL_DEBUG_TYPE_PERFORMANCE: return "DEBUG_TYPE_PERFORMANCE";
    case GL_DEBUG_TYPE_MARKER: return "DEBUG_TYPE_MARKER";
    case GL_DEBUG_TYPE_PUSH_GROUP: return "DEBUG_TYPE_PUSH_GROUP";
    case GL_DEBUG_TYPE_POP_GROUP: return "DEBUG_TYPE_POP_GROUP";
    case GL_DEBUG_TYPE_OTHER: return "DEBUG_TYPE_OTHER";

    default: return "UNKNOWN_DEBUG_TYPE";
    }
}

RenderOpenGL::RenderOpenGL(Window* pWindow) 
    : m_pWindow(pWindow), m_count(++RenderOpenGL::m_renderCount)
{
    LINFO("[Render {}]\tRenderOpenGL::RenderOpenGL(\"{}\")", this->m_count, pWindow->get_title()->c_str());

    int code = this->initialize();
    LINFO("[Render {}]\tinit code {} ({})", this->m_count, code, RenderOpenGL::returnsToString(code));
}

int RenderOpenGL::initialize()
{
    LINFO("[Render {}]\tRenderOpenGL::initialize()", this->m_count);
    this->m_pWindow->make_contextActive();

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        LCRITICAL("[Window {}]\tCan't init GLAD", this->m_count);
        return RenderOpenGL::Returns::CANT_INIT_GLAD;
    }

    LINFO("OpenGL context initialized:");
    LINFO("  OpenGL Vendor: {0}", get_vendor_str());
    LINFO("  OpenGL Renderer: {0}", get_renderer_str());
    LINFO("  OpenGL Version: {0}", get_version_str());

    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);
    glDebugMessageCallback([](GLenum source,
        GLenum type,
        GLuint id,
        GLenum severity,
        GLsizei length,
        const GLchar* message,
        const void* userParam)
        {
            switch (severity)
            {
            case GL_DEBUG_SEVERITY_HIGH:
                LERROR("OpenGL Error: [{0}:{1}]({2}): {3}",
                    gl_source_to_string(source),
                    gl_type_to_string(type),
                    id,
                    message);
                break;
            case GL_DEBUG_SEVERITY_MEDIUM:
                if (id == 131218) break; // state performance warning: Fragment Shader is going to be recompiled

                LERROR("OpenGL Warning: [{0}:{1}]({2}): {3}",
                    gl_source_to_string(source),
                    gl_type_to_string(type),
                    id,
                    message);
                break;
            case GL_DEBUG_SEVERITY_LOW:
                LERROR("OpenGL Info: [{0}:{1}]({2}): {3}",
                    gl_source_to_string(source),
                    gl_type_to_string(type),
                    id,
                    message);
                break;
            case GL_DEBUG_SEVERITY_NOTIFICATION:
                LERROR("OpenGL Notificaton: [{0}:{1}]({2}): {3}",
                    gl_source_to_string(source),
                    gl_type_to_string(type),
                    id,
                    message);
                break;
            default:
                LERROR("OpenGL Error: [{0}:{1}] ({2}) : {3}",
                    gl_source_to_string(source),
                    gl_type_to_string(type),
                    id,
                    message);
            }
        }, nullptr);

    return RenderOpenGL::Returns::NO_ERRORS;
}

void RenderOpenGL::draw(const VAO& vao)
{
    this->m_pWindow->make_contextActive();
    vao.bind();
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(vao.get_indicesCount()), GL_UNSIGNED_INT, nullptr);
}

void RenderOpenGL::set_clear_color(const float r, const float g, const float b, const float a)
{
    this->m_pWindow->make_contextActive();
    glClearColor(r, g, b, a);
}

void RenderOpenGL::clear()
{
    this->m_pWindow->make_contextActive();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RenderOpenGL::set_viewport(const unsigned int width, const unsigned int height, const unsigned int left_offset, const unsigned int bottom_offset)
{
    this->m_pWindow->make_contextActive();
    glViewport(left_offset, bottom_offset, width, height);
}

void RenderOpenGL::enable_depth_test()
{
    this->m_pWindow->make_contextActive();
    glEnable(GL_DEPTH_TEST);
}

void RenderOpenGL::disable_depth_test()
{
    this->m_pWindow->make_contextActive();
    glDisable(GL_DEPTH_TEST);
}

const char* RenderOpenGL::get_vendor_str()
{
    return reinterpret_cast<const char*>(glGetString(GL_VENDOR));
}

const char* RenderOpenGL::get_renderer_str()
{
    return reinterpret_cast<const char*>(glGetString(GL_RENDERER));
}

const char* RenderOpenGL::get_version_str()
{
    return reinterpret_cast<const char*>(glGetString(GL_VERSION));
}

double RenderOpenGL::getTime() { return glfwGetTime(); }

void RenderOpenGL::finalize()      {                   }
     RenderOpenGL::~RenderOpenGL() { this->finalize(); }
