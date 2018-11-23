#pragma once

#include <windows.h>
#include <commctrl.h>
#include <functional>
#include <vector>
#include "element.h"
#include "../callback_queue.h"

namespace UI
{
	class ListView : public Element
	{
	public:
		ListView(HINSTANCE, HFONT, HWND, int, int, int, int, std::function<void()>);
		void AddColumn(char *, int);
		void AddColumn(char const *, int);
		void InsertItem(char *);
		void InsertItem(char const *);
		void ClearItems();
		std::vector<int> GetSelection();
		LRESULT OnMessage(UINT, WPARAM, LPARAM) override;
		std::function<void()> OnSelectionChanged;

	private:
		int NextSelectedItem(int prev = -1);
		int columnsCount, itemsCount;
	};
}