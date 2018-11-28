#include "list_view.h"

UI::ListView::ListView(HINSTANCE instance, HFONT font, HWND parent, int x, int y, int width, int height, const char *caption,
	std::function<void()> onSelectionChanged)
{
	INITCOMMONCONTROLSEX icex;
	icex.dwICC = ICC_LISTVIEW_CLASSES;
	InitCommonControlsEx(&icex);

	this->OnSelectionChanged = onSelectionChanged;
	this->handle = CreateWindowEx(WS_EX_CLIENTEDGE, WC_LISTVIEW, NULL, WS_CHILD | WS_VISIBLE | LVS_REPORT |
		LVS_SHOWSELALWAYS | LVS_OWNERDATA, x, y, width, height, parent, NULL, instance, NULL);

	if (!this->handle)
		throw Exception("CreateWindow failed");

	this->InitializeElement(font);
	SendMessage(this->handle, LVM_SETEXTENDEDLISTVIEWSTYLE, LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);

	char *caption—opy = _strdup(caption);
	LVCOLUMN column;
	column.mask = LVCF_TEXT | LVCF_WIDTH;
	column.pszText = caption—opy;
	column.cx = width - 4;
	SendMessage(this->handle, LVM_INSERTCOLUMN, NULL, (LPARAM)&column);
	free(caption—opy);
}

void UI::ListView::SetDataSource(std::vector<std::string> &dataSource)
{
	this->dataSource.reserve(this->dataSource.size() + dataSource.size());
	this->dataSource.insert(this->dataSource.end(), dataSource.begin(), dataSource.end());
	SendMessage(this->handle, LVM_SETITEMCOUNT, this->dataSource.size(), NULL);
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
	this->dataSource.clear();
	SendMessage(this->handle, LVM_DELETEALLITEMS, NULL, NULL);
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
		switch (((NMHDR *)lParam)->code)
		{
		case LVN_GETDISPINFO:
		{
			auto info = (NMLVDISPINFO *)lParam;
			info->item.pszText = &this->dataSource[info->item.iItem][0];
		}
		break;

		case LVN_ODSTATECHANGED:
			if (this->OnSelectionChanged != nullptr)
			{
				auto info = (NMLVODSTATECHANGE *)lParam;
				if ((info->uNewState ^ info->uOldState) & LVIS_SELECTED)
					CallbackQueue::GetInstance()->Push(this->OnSelectionChanged);
			}
			break;

		case LVN_ITEMCHANGED:
			if (this->OnSelectionChanged != nullptr)
			{
				auto info = (NMLISTVIEW *)lParam;
				if ((info->uNewState ^ info->uOldState) & LVIS_SELECTED)
					CallbackQueue::GetInstance()->Push(this->OnSelectionChanged);
			}
			break;
		}

		return 0;
	}

	return UI::Element::OnMessage(message, wParam, lParam);
}
