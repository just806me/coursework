#pragma once

#include <windows.h>
#include <functional>
#include "../exception.h"
#include "../callback_queue.h"
#include "element.h"

namespace UI
{
	class Window : public Element
	{
	public:
		Window(HINSTANCE, HFONT, HICON, const char *, int, int, HMENU, std::function<void(WORD)>);
		~Window() override;

		void Show();
		void Close();
		void StartMessageLoop();

		std::function<void(WORD)> OnMenuSelected;

	protected:
		LRESULT OnMessage(UINT, WPARAM, LPARAM) override;

	private:
		const char *className;
		HINSTANCE instance;
	};
}
