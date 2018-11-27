#pragma once

#include <windows.h>
#include <functional>
#include "element.h"
#include "../callback_queue.h"

namespace UI
{
	class NumberEdit : public Element
	{
	public:
		NumberEdit(HINSTANCE, HFONT, HWND, int, int, int, int, std::function<void()>);
		std::string GetText();
		void SetText(const char *, bool notify = false);

		std::function<void()> OnChanged;

	protected:
		LRESULT OnMessage(UINT, WPARAM, LPARAM) override;

	private:
		bool isChangingText;
	};
}
