#pragma once

#include <windows.h>
#include <vector>
#include <string>
#include "file.h"

class Model
{
public:
	void Add(std::vector<std::string> &);
	void Clear();
	bool Save();
	void Restore();
	void Set(DWORD, DWORD);
	void Set(DWORD);
	void SetSelection(std::vector<int> &&);
	DWORD GetSetMask();
	DWORD GetUnsetMask();
	bool IsSelectionEmpty();

private:
	std::vector<File> files;
	std::vector<int> selection;
};
