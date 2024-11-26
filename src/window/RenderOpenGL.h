#pragma once

#include <string>
#include <graphics/buffers/VAO.h>

struct Window;

class RenderOpenGL
{
	static unsigned short m_renderCount;
	       unsigned short m_count = 0;
	
	Window* m_pWindow = nullptr;

	int initialize();
	void finalize();

public:
	enum Returns
	{
		NO_ERRORS      =  0,
		CANT_INIT_GLAD = -1,
	};

	static constexpr std::string returnsToString(int code)
	{
		switch (code)
		{
		case RenderOpenGL::Returns::NO_ERRORS:
			return "NO_ERRORS";
			break;
		case RenderOpenGL::Returns::CANT_INIT_GLAD:
			return "CANT_INIT_GLAD";
			break;
		} return "UNKNOWN_TYPE";
	}

	RenderOpenGL(Window* pWindow);
	~RenderOpenGL();

	RenderOpenGL(const RenderOpenGL& ) = delete;
	RenderOpenGL(      RenderOpenGL&&) = delete;

	RenderOpenGL& operator=(const RenderOpenGL& ) = delete;
	RenderOpenGL& operator=(      RenderOpenGL&&) = delete;

	void draw(const VAO& vao);
	void set_clear_color(const float r, const float g, const float b, const float a);
	void clear();
	void set_viewport(const unsigned int width, const unsigned int height, const unsigned int left_offset = 0, const unsigned int bottom_offset = 0);
	void enable_depth_test();
	void disable_depth_test();

	const char* get_vendor_str();
	const char* get_renderer_str();
	const char* get_version_str();

	double getTime();
};

