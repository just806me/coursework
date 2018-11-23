#include "checkbox.h"

UI::Checkbox::Checkbox(HINSTANCE instance, HFONT font, HWND parent, const char *text, int x, int y, int width, int height,
	std::function<void()> onChanged)
{
	this->handle = CreateWindowEx(NULL, "BUTTON", text, WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_AUTO3STATE,
		x, y, width, height, parent, NULL, NULL, NULL);
	this->OnChanged = onChanged;

	if (!this->handle)
		throw new Exception("CreateWindow failed");

	this->InitializeElement(font);
}

DWORD UI::Checkbox::GetState()
{
	return SendMessage(this->handle, BM_GETCHECK, NULL, NULL);
}

void UI::Checkbox::SetState(DWORD newState)
{
	if (newState != BST_CHECKED && newState != BST_INDETERMINATE && newState != BST_UNCHECKED)
		throw new Exception("Bad checkbox state");

	SendMessage(this->handle, BM_SETCHECK, newState, NULL);
}

LRESULT UI::Checkbox::OnMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_COMMAND)
	{
		if (this->OnChanged != nullptr)
			CallbackQueue::GetInstance()->Push(this->OnChanged);

		return 0;
	}

	return UI::Element::OnMessage(message, wParam, lParam);
}
