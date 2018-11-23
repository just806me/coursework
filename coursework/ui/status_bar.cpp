#include "status_bar.h"

UI::StatusBar::StatusBar(HINSTANCE instance, HFONT font, HWND parent, const char *text)
{
	this->handle = CreateWindowEx(NULL, STATUSCLASSNAME, text, WS_CHILD | WS_VISIBLE,
		0, 0, 0, 0, parent, NULL, instance, NULL);

	if (!this->handle)
		throw Exception("CreateWindow failed");

	this->InitializeElement(font);
}

void UI::StatusBar::SetText(const char *text)
{
	SendMessage(this->handle, SB_SETTEXT, 0, (LPARAM)text);
}
