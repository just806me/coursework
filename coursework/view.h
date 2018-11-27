#pragma once

#include <windows.h>
#include <mshtmcid.h>
#include <mutex>
#include "ui/prelude.h"
#include "resource.h"
#include "model.h"

class View
{
public:
	View(HINSTANCE);
	~View();

	void Run();
	void Update();
	void SetEnabled(bool);

private:
	Model model;

	HINSTANCE instance;
	HFONT     font;
	HMENU     menu;
	HICON     icon;

	UI::Window     *window;
	UI::StatusBar  *statusBar;
	UI::Toolbar    *toolbar;
	UI::GroupBox   *groupBox1, *groupBox2;
	UI::ListView   *filesListView;
	UI::NumberEdit *attributeEdit;
	UI::Button     *buttonApply;
	std::unique_ptr<UI::Checkbox> *checkboxes;

	void OnMenuSelected(WORD);
	void OnButtonApplyClick();
	void OnAttributeEditChange();
	void OnFilesListViewSelectionChanged();
	std::function<void()> OnCheckboxChanged(size_t);
	static INT_PTR CALLBACK DialogFunc(HWND, UINT, WPARAM, LPARAM);
};