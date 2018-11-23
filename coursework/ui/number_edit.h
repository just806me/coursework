#pragma once

#include <windows.h>
#include "element.h"

namespace UI
{
	class NumberEdit : public Element
	{
	public:
		NumberEdit(HINSTANCE, HFONT, HWND, int, int, int, int);
		std::string GetText();
		void SetText(const char *);
	};
}
