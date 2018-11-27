#include "list_view.h"

UI::ListView::ListView(HINSTANCE instance, HFONT font, HWND parent, int x, int y, int width, int height,
	std::function<void()> onSelectionChanged)
{
	INITCOMMONCONTROLSEX icex;
	icex.dwICC = ICC_LISTVIEW_CLASSES;
	InitCommonControlsEx(&icex);

	this->OnSelectionChanged = onSelectionChanged;
	this->columnsCount = this->itemsCount = 0;
	this->handle = CreateWindowEx(WS_EX_CLIENTEDGE, WC_LISTVIEW, NULL, WS_CHILD | WS_VISIBLE
		| LVS_REPORT | LVS_SHOWSELALWAYS, x, y, width, height, parent, NULL, instance, NULL);

	if (!this->handle)
		throw Exception("CreateWindow failed");

	this->InitializeElement(font);
	SendMessage(this->handle, LVM_SETEXTENDEDLISTVIEWSTYLE, LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);

	//NOTIFY_HANDLER(IDC_FILELIST, LVN_ITEMCHANGED, OnListViewItemChanged)
}

void UI::ListView::AddColumn(char const *caption, int width)
{
	char *copy = _strdup(caption);
	AddColumn(copy, width);
	free(copy);
}

void UI::ListView::InsertItem(char const *text)
{
	char *copy = _strdup(text);
	InsertItem(copy);
	free(copy);
}

void UI::ListView::InsertItem(char *text)
{
	LVITEM item;
	item.iItem = this->columnsCount++;
	item.iSubItem = 0;
	item.mask = LVIF_TEXT;
	item.pszText = text;
	SendMessage(this->handle, LVM_INSERTITEM, 0, (LPARAM)&item);
}

void UI::ListView::AddColumn(char *caption, int width)
{
	LVCOLUMN column;
	column.mask = LVCF_TEXT | LVCF_WIDTH;
	column.pszText = caption;
	column.cx = width;
	SendMessage(this->handle, LVM_INSERTCOLUMN, this->columnsCount++, (LPARAM)&column);
}

void UI::ListView::SelectAll()
{
	LVITEM item;
	item.mask = LVIF_STATE;
	item.stateMask = LVIS_SELECTED;
	item.state = LVNI_SELECTED;
	SendMessage(this->handle, LVM_SETITEMSTATE, -1, (LPARAM)&item);
}

void UI::ListView::Deselect()
{
	LVITEM item;
	item.mask = LVIF_STATE;
	item.stateMask = LVIS_SELECTED;
	item.state = 0;
	SendMessage(this->handle, LVM_SETITEMSTATE, -1, (LPARAM)&item);
}

void UI::ListView::ClearItems()
{
	SendMessage(this->handle, LVM_DELETEALLITEMS, NULL, NULL);
	this->itemsCount = 0;
}

std::vector<int> UI::ListView::GetSelection()
{
	std::vector<int> result;

	for (int i = this->NextSelectedItem(); i != -1; i = this->NextSelectedItem(i))
		result.push_back(i);

	return result;
}

int UI::ListView::NextSelectedItem(int prev)
{
	return SendMessage(this->handle, LVM_GETNEXTITEM, prev, LVNI_SELECTED);
}

LRESULT UI::ListView::OnMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_NOTIFY)
	{
		if (this->OnSelectionChanged != nullptr)
		{
			NMLISTVIEW *info = (NMLISTVIEW *)lParam;
			if (info->hdr.code == LVN_ITEMCHANGED
				&& ((info->uNewState & LVIS_SELECTED) == LVIS_SELECTED || (info->uOldState & LVIS_SELECTED) == LVIS_SELECTED))
				CallbackQueue::GetInstance()->Push(this->OnSelectionChanged);
		}

		return 0;
	}

	return UI::Element::OnMessage(message, wParam, lParam);
}
