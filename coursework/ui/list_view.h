#pragma once

#include <windows.h>
#include <commctrl.h>
#include <functional>
#include <vector>
#include <string>
#include "element.h"
#include "../callback_queue.h"

namespace UI
{
	class ListView : public Element
	{
	public:
		ListView(HINSTANCE, HFONT, HWND, int, int, int, int, const char *, std::function<void()>);
		void SetDataSource(std::vector<std::string> &);
		void SelectAll();
		void Deselect();
		void ClearItems();
		std::vector<int> GetSelection();
		LRESULT OnMessage(UINT, WPARAM, LPARAM) override;
		std::function<void()> OnSelectionChanged;

	private:
		int NextSelectedItem(int prev = -1);
		std::vector<std::string> dataSource;
	};
}