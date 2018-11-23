#include "number_edit.h"

UI::NumberEdit::NumberEdit(HINSTANCE instance, HFONT font, HWND parent, int x, int y, int width, int height)
{
	this->handle = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", NULL, WS_CHILD | WS_VISIBLE | ES_LEFT | ES_NUMBER,
		x, y, width, height, parent, NULL, instance, NULL);
	if (!this->handle)
		throw new Exception("CreateWindow failed");
	this->InitializeElement(font);
}

std::string UI::NumberEdit::GetText()
{
	const size_t size = SendMessage(this->handle, WM_GETTEXTLENGTH, NULL, NULL) + 1;
	std::string result;
	result.resize(size + 1);
	SendMessage(this->handle, WM_GETTEXT, size, (LPARAM)result.c_str());
	return result;
}

void UI::NumberEdit::SetText(const char *text)
{
	SendMessage(this->handle, WM_SETTEXT, 0, (LPARAM)text);
}
