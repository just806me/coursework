#include "open_file_dialog.h"

UI::OpenFileDialog::OpenFileDialog(HINSTANCE instance, HWND parent, const char *title)
{
	this->buffer = new char[UI::OpenFileDialog::BUFFER_SIZE];
	this->ofn.lStructSize = sizeof(OPENFILENAME);
	this->ofn.hwndOwner = parent;
	this->ofn.hInstance = instance;
	this->ofn.lpstrFilter = NULL;
	this->ofn.lpstrCustomFilter = NULL;
	this->ofn.nFilterIndex = NULL;
	this->ofn.lpstrFile = this->buffer;
	this->ofn.nMaxFile = UI::OpenFileDialog::BUFFER_SIZE;
	this->ofn.lpstrFileTitle = NULL;
	this->ofn.lpstrInitialDir = NULL;
	this->ofn.lpstrTitle = title;
	this->ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_FORCESHOWHIDDEN | OFN_NODEREFERENCELINKS | OFN_NONETWORKBUTTON | OFN_ALLOWMULTISELECT | OFN_EXPLORER;
	this->ofn.lpstrDefExt = NULL;
}

UI::OpenFileDialog::~OpenFileDialog()
{
	delete[] this->buffer;
}

bool UI::OpenFileDialog::Show()
{
	memset(this->buffer, 0, UI::OpenFileDialog::BUFFER_SIZE);
	return GetOpenFileName(&this->ofn);
}

std::vector<std::string> UI::OpenFileDialog::GetSelectedFiles()
{
	std::string path(this->buffer);
	size_t offset = path.size() + 1;

	if (this->buffer[offset] == 0)
		return { std::move(path) };

	std::vector<std::string> result;

	while (this->buffer[offset] != 0)
	{
		std::string name(this->buffer + offset);
		result.push_back(path + '\\' + name);
		offset += name.size() + 1;
	}

	return result;
}
