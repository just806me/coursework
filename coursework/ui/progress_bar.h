#pragma once

#include <windows.h>
#include <commctrl.h>
#include "element.h"

namespace UI
{
	class ProgressBar : public Element
	{
	public:
		ProgressBar(HINSTANCE, HFONT, HWND, int, int, int, int);
		void SetRange(size_t);
		void Step();
	};
}
