#pragma once

#include <windows.h>
#include "element.h"

namespace UI
{
	class GroupBox : public Element
	{
	public:
		GroupBox(HINSTANCE, HFONT, HWND, int, int, int, int, const char *);

	private:
		static WNDPROC oldWndProc;
		static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
	};
}