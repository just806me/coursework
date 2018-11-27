#include "number_edit.h"

UI::NumberEdit::NumberEdit(HINSTANCE instance, HFONT font, HWND parent, int x, int y, int width, int height,
	std::function<void()> onChanged)
{
	this->handle = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", NULL, WS_CHILD | WS_VISIBLE | ES_LEFT | ES_NUMBER,
		x, y, width, height, parent, NULL, instance, NULL);

	if (!this->handle)
		throw new Exception("CreateWindow failed");

	this->OnChanged = onChanged;
	this->isChangingText = false;
	this->InitializeElement(font);
}

std::string UI::NumberEdit::GetText()
{
	const size_t size = SendMessage(this->handle, WM_GETTEXTLENGTH, NULL, NULL);
	std::string result;
	result.resize(size);
	SendMessage(this->handle, WM_GETTEXT, size + 1, (LPARAM)&result[0]);
	return result;
}

void UI::NumberEdit::SetText(const char *text, bool notify)
{
	if (!notify)
		this->isChangingText = true;

	int len = strlen(text);
	SendMessage(this->handle, WM_SETTEXT, 0, (LPARAM)text);
	SendMessage(this->handle, EM_SETSEL, len, len);
}

LRESULT UI::NumberEdit::OnMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_COMMAND && HIWORD(wParam) == EN_CHANGE)
	{
		if (this->isChangingText)
			this->isChangingText = false;
		else if (this->OnChanged != nullptr)
			CallbackQueue::GetInstance()->Push(this->OnChanged);

		return 0;
	}

	return UI::Element::OnMessage(message, wParam, lParam);
}
