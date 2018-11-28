#pragma once

#include <windows.h>
#include <vector>
#include <string>
#include <functional>
#include "file.h"

class Model
{
public:
	void Add(std::string &);
	void Clear();
	bool Save(std::function<void()>);
	void Restore();
	void Set(DWORD, DWORD);
	void Set(DWORD);
	void SetSelection(std::vector<int> &&);
	DWORD GetSetMask();
	DWORD GetUnsetMask();
	bool IsSelectionEmpty();
	size_t GetSelectionSize();

private:
	std::vector<File> files;
	std::vector<int> selection;
};
