#include "group_box.h"

WNDPROC UI::GroupBox::oldWndProc = NULL;

UI::GroupBox::GroupBox(HINSTANCE instance, HFONT font, HWND parent, int x, int y, int width, int height, const char *text)
{
	this->handle = CreateWindowEx(NULL, "BUTTON", text, WS_VISIBLE | WS_CHILD | BS_GROUPBOX | WS_GROUP,
		x, y, width, height, parent, NULL, instance, NULL);

	if (!this->handle)
		throw Exception("CreateWindow failed");

	GroupBox::oldWndProc = (WNDPROC)GetWindowLongPtr(this->handle, GWLP_WNDPROC);
	SetWindowLongPtr(this->handle, GWLP_WNDPROC, (LONG_PTR)GroupBox::WndProc);

	this->InitializeElement(font);
}

LRESULT UI::GroupBox::WndProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_COMMAND)
		CallWindowProc(Element::WndProc, GetParent(handle), message, wParam, lParam);

	return CallWindowProc(GroupBox::oldWndProc, handle, message, wParam, lParam);
}
