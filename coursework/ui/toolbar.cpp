#include "toolbar.h"

UI::Toolbar::Toolbar(HINSTANCE instance, HFONT font, HWND parent, TBBUTTON *buttons, int buttonsCount,
	std::function<void(WORD)> onSelected)
{
	this->handle = CreateWindowEx(WS_EX_CLIENTEDGE, TOOLBARCLASSNAME, NULL, WS_CHILD | TBSTYLE_WRAPABLE, 0, 0, 0, 0,
		parent, NULL, instance, NULL);

	if (!this->handle)
		throw Exception("CreateWindow failed");

	HIMAGELIST images = ImageList_Create(16, 16, ILC_COLOR16 | ILC_MASK, buttonsCount, 0);
	this->OnSelected = onSelected;
	this->InitializeElement(font);

	SendMessage(this->handle, TB_SETIMAGELIST, NULL, (LPARAM)images);
	SendMessage(this->handle, TB_LOADIMAGES, IDB_STD_SMALL_COLOR, (LPARAM)HINST_COMMCTRL);
	SendMessage(this->handle, TB_BUTTONSTRUCTSIZE, sizeof(TBBUTTON), NULL);
	SendMessage(this->handle, TB_ADDBUTTONS, buttonsCount, (LPARAM)buttons);
	SendMessage(this->handle, TB_AUTOSIZE, NULL, NULL);

	ShowWindow(this->handle, TRUE);
}

LRESULT UI::Toolbar::OnMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_COMMAND)
	{
		if (this->OnSelected != nullptr)
			CallbackQueue::GetInstance()->Push([this, wParam]() { this->OnSelected(LOWORD(wParam)); });

		return 0;
	}

	return UI::Element::OnMessage(message, wParam, lParam);
}
