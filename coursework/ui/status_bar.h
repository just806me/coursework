#pragma once

#include <windows.h>
#include <commctrl.h>
#include "element.h"

namespace UI
{
	class StatusBar : public Element
	{
	public:
		StatusBar(HINSTANCE, HFONT, HWND, const char *);
		void SetText(const char *);
	};
}
