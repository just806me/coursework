#include "view.h"

View::View(HINSTANCE instance)
{
	TBBUTTON toolbarButtons[] = {
		{ MAKELONG(STD_FILEOPEN, NULL), IDM_OPEN, TBSTATE_ENABLED, BTNS_AUTOSIZE, {0}, 0, NULL },
		{ MAKELONG(STD_FILESAVE, NULL), IDM_SAVE, TBSTATE_ENABLED, BTNS_AUTOSIZE, {0}, 0, NULL },
		{ MAKELONG(STD_UNDO, NULL), IDM_UNDO, TBSTATE_ENABLED, BTNS_AUTOSIZE, {0}, 0, NULL },
		{ MAKELONG(STD_DELETE, NULL), IDM_DELETE, TBSTATE_ENABLED, BTNS_AUTOSIZE, {0}, 0, NULL }
	};

	this->instance = instance;

	this->menu = LoadMenu(NULL, MAKEINTRESOURCE(IDR_MENU1));

	this->icon = LoadIcon(this->instance, MAKEINTRESOURCE(IDI_ICON2));

	this->font = CreateFont(16, 0, 0, 0, 400, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
		CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, "Segoe UI");

	this->window = new UI::Window(this->instance, this->font, this->icon, "Менеджер атрибутів", 800, 608, this->menu,
		[this](WORD item) { this->OnMenuSelected(item); });

	this->statusBar = new UI::StatusBar(this->instance, this->font, window->GetHandle(), "ОК");

	this->progressBar = new UI::ProgressBar(this->instance, this->font, this->statusBar->GetHandle(), 586, 2, 198, 20);

	this->toolbar = new UI::Toolbar(this->instance, this->font, window->GetHandle(), toolbarButtons,
		_countof(toolbarButtons), [this](WORD item) { this->OnMenuSelected(item); });

	this->groupBox1 = new UI::GroupBox(this->instance, this->font, window->GetHandle(), 588, 30, 194, 422,
		"Задати атрибути візуально");

	this->groupBox2 = new UI::GroupBox(this->instance, this->font, window->GetHandle(), 588, 452, 194, 48,
		"Задати атрибути числом");

	this->buttonApply = new UI::Button(this->instance, this->font, window->GetHandle(), "Застосувати",
		588, 502, 194, 22, [this]() { this->OnButtonApplyClick(); });

	this->attributeEdit = new UI::NumberEdit(this->instance, this->font, groupBox2->GetHandle(), 4, 20, 186, 22,
		[this]() { this->OnAttributeEditChange(); });

	this->filesListView = new UI::ListView(this->instance, this->font, window->GetHandle(), 2, 30, 584, 496, "Файл",
		[this]() { this->OnFilesListViewSelectionChanged(); });

	this->checkboxes = new std::unique_ptr<UI::Checkbox>[File::ATTRIBUTES_COUNT];

	for (size_t i = 0; i < File::ATTRIBUTES_COUNT; i++)
	{
		auto checkbox = new UI::Checkbox(instance, font, groupBox1->GetHandle(), File::ATTRIBUTES[i].second,
			4, 20 + (i * 25), 180, 25, this->OnCheckboxChanged(i));
		checkboxes[i] = std::unique_ptr<UI::Checkbox>(checkbox);
	}
}

View::~View()
{
	delete buttonApply;
	delete attributeEdit;
	delete[] checkboxes;
	delete filesListView;
	delete groupBox2;
	delete groupBox1;
	delete toolbar;
	delete statusBar;
	delete window;

	DestroyMenu(this->menu);
	DestroyIcon(this->icon);
	DeleteObject(this->font);
}

void View::Run()
{
	this->Update();
	window->Show();
	window->StartMessageLoop();
}

INT_PTR View::DialogFunc(HWND handle, UINT message, WPARAM wParam, LPARAM)
{
	if (message == WM_INITDIALOG)
		return 1;

	if (message == WM_COMMAND && (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL))
	{
		EndDialog(handle, LOWORD(wParam));
		return 1;
	}

	return 0;
}

void View::OnMenuSelected(WORD item)
{
	switch (item)
	{
	case IDM_OPEN:
	case ID_FILE_ADD:
	{
		this->statusBar->SetText("Вибір файлу(ів) ...");
		UI::OpenFileDialog dialog(instance, window->GetHandle(), "Виберіть файл(и)");

		this->SetEnabled(false);
		this->filesListView->SetEnabled(false);
		this->toolbar->SetEnabled(false);

		if (dialog.Show())
		{
			this->statusBar->SetText("Завантаження файлу(ів) ...");

			auto files = dialog.GetSelectedFiles();
			this->progressBar->SetRange(files.size());
			this->filesListView->SetDataSource(files);

			for (auto &file : files)
			{
				this->model.Add(file);

				this->progressBar->Step();
				Sleep(1);
			}
		}

		this->filesListView->SetEnabled(true);
		this->toolbar->SetEnabled(true);
		this->Update();

		this->statusBar->SetText("ОК");
	}
	break;

	case IDM_DELETE:
	case ID_FILE_CLOSE_ALL:
		this->model.Clear();
		this->filesListView->ClearItems();
		this->Update();
		break;

	case ID_FILE_EXIT:
		this->window->Close();
		break;

	case IDM_SAVE:
	case ID_EDIT_APPLY:
		this->OnButtonApplyClick();
		break;

	case IDM_UNDO:
	case ID_EDIT_UNDO:
		this->statusBar->SetText("Відміна змін ...");
		this->model.Restore();
		this->Update();
		this->statusBar->SetText("ОК");
		break;

	case ID_EDIT_SELECT_ALL:
		this->filesListView->SelectAll();
		this->Update();
		break;

	case ID_EDIT_DESELECT:
		this->filesListView->Deselect();
		this->Update();
		break;

	case ID_ABOUT:
		DialogBox(instance, MAKEINTRESOURCE(IDD_DIALOG1), this->window->GetHandle(), View::DialogFunc);
		break;

	case ID_HELP:
		ShellExecute(this->window->GetHandle(), "open", "help.chm", NULL, NULL, SW_SHOW);
		break;

	default:
		MessageBox(NULL, "Unimplemented!", "Information", MB_OK | MB_ICONINFORMATION);
		break;
	}
}

void View::OnButtonApplyClick()
{
	this->statusBar->SetText("Збереження змін ...");
	this->progressBar->SetRange(this->model.GetSelectionSize());

	this->filesListView->SetEnabled(false);
	this->toolbar->SetEnabled(false);
	this->SetEnabled(false);

	bool result = this->model.Save([this]() { this->progressBar->Step(); });

	this->filesListView->SetEnabled(true);
	this->toolbar->SetEnabled(true);
	this->Update();

	this->statusBar->SetText(result ? "ОК" : "Виникла помилка при збереженні змін");
}

void View::OnAttributeEditChange()
{
	std::string text = this->attributeEdit->GetText();
	if (text.size() > 0)
	{
		this->statusBar->SetText("Модифіковано");
		DWORD value;
		sscanf_s(text.c_str(), "%lu", &value);
		this->model.Set(value);
		this->Update();
	}
}

std::function<void()> View::OnCheckboxChanged(size_t i)
{
	return [this, i]() {
		switch (this->checkboxes[i]->GetState()) {
		case BST_CHECKED:
		case BST_INDETERMINATE:
			this->model.Set(File::ATTRIBUTES[i].first, BST_UNCHECKED);
			break;

		case BST_UNCHECKED:
			this->model.Set(File::ATTRIBUTES[i].first, BST_CHECKED);
			break;
		}

		this->statusBar->SetText("Модифіковано");
		this->Update();
	};
}

void View::SetEnabled(bool value)
{
	this->attributeEdit->SetEnabled(value);
	this->buttonApply->SetEnabled(value);
	for (size_t i = 0; i < File::ATTRIBUTES_COUNT; i++)
		this->checkboxes[i]->SetEnabled(value);
}

void View::OnFilesListViewSelectionChanged()
{
	this->model.SetSelection(this->filesListView->GetSelection());
	this->Update();
}

void View::Update()
{
	if (this->model.IsSelectionEmpty())
		this->SetEnabled(false);
	else
	{
		DWORD set = this->model.GetSetMask(), unset = this->model.GetUnsetMask();
		for (size_t i = 0; i < File::ATTRIBUTES_COUNT; i++)
		{
			DWORD attribute = File::ATTRIBUTES[i].first;

			if ((attribute & set) == attribute)
				this->checkboxes[i]->SetState(BST_CHECKED);
			else if ((attribute & unset) == attribute)
				this->checkboxes[i]->SetState(BST_UNCHECKED);
			else
				this->checkboxes[i]->SetState(BST_INDETERMINATE);
		}

		char buffer[12] = { 0 };
		sprintf_s(buffer, "%lu", set);
		this->attributeEdit->SetText(buffer);

		this->SetEnabled(true);
	}
}
