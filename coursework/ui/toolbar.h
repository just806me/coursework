#pragma once

#include <functional>
#include <windows.h>
#include <commctrl.h>
#include <mshtmcid.h>
#include "element.h"
#include "../callback_queue.h"

namespace UI
{
	class Toolbar : public Element
	{
	public:
		Toolbar(HINSTANCE, HFONT, HWND, TBBUTTON *, int, std::function<void(WORD)>);
		std::function<void(WORD)> OnSelected;

	protected:
		LRESULT OnMessage(UINT message, WPARAM wParam, LPARAM lParam) override;
	};
}
