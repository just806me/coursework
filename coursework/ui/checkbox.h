#pragma once

#include <windows.h>
#include <functional>
#include "element.h"
#include "../callback_queue.h"

namespace UI
{
	class Checkbox : public Element
	{
	public:
		Checkbox(HINSTANCE, HFONT, HWND, const char *, int, int, int, int, std::function<void()>);
		DWORD GetState();
		void SetState(DWORD);

		std::function<void()> OnChanged;

	protected:
		LRESULT OnMessage(UINT, WPARAM, LPARAM) override;
	};
}
