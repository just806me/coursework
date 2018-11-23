#include "button.h"

UI::Button::Button(HINSTANCE instance, HFONT font, HWND parent, const char *text, int x, int y, int width, int height,
	std::function<void()> onClick)
{
	this->handle = CreateWindowEx(NULL, "BUTTON", text, WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
		x, y, width, height, parent, NULL, instance, NULL);
	this->OnClick = onClick;

	if (!this->handle)
		throw Exception("CreateWindow failed");

	this->InitializeElement(font);
}

LRESULT UI::Button::OnMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_COMMAND)
	{
		if (this->OnClick != nullptr)
			CallbackQueue::GetInstance()->Push(this->OnClick);

		return 0;
	}

	return UI::Element::OnMessage(message, wParam, lParam);
}
