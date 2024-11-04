#pragma once

#include <iostream>
#include <string>

struct ImGuiContext;
class Window;

class UI
{
	static unsigned short m_uisCount;
	       unsigned short m_count = 0;

	ImGuiContext* m_pContext       = nullptr;
	Window*       m_pWindow        = nullptr;

	int initialize();
	void finalize();

public:
	enum Returns
	{
		NO_ERRORS = 0,
	};

	static constexpr std::string returnsToString(int code)
	{
		switch (code)
		{
		case UI::Returns::NO_ERRORS:
			return "NO_ERRORS";
			break;
		} return "UNKNOWN_TYPE";
	}

	UI(Window* pWindow);
	~UI();

	UI(const UI& ) = delete;
	UI(      UI&&) = delete;

	UI& operator=(const UI& ) = delete;
	UI& operator=(      UI&&) = delete;

	void updateBegin();
	void updateEnd();

	void make_contextActive();

	unsigned short get_count() const { return this->m_count;   }
	Window* const get_window() const { return this->m_pWindow; }
};

