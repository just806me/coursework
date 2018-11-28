#include "model.h"

void Model::Add(std::string &file)
{
	this->files.push_back(file);
}

void Model::Clear()
{
	this->files.clear();
	this->selection.clear();
}

bool Model::Save(std::function<void()> progressCallback)
{
	bool result = true;

	for (auto i : this->selection)
	{
		result &= this->files[i].Save();
		progressCallback();
		Sleep(1);
	}

	return result;
}

void Model::Restore()
{
	for (auto i : this->selection)
		this->files[i].Restore();
}

void Model::Set(DWORD attribute, DWORD value)
{
	for (auto i : this->selection)
		this->files[i].Set(attribute, value);
}

void Model::Set(DWORD value)
{
	for (auto i : this->selection)
		this->files[i].Set(value);
}

void Model::SetSelection(std::vector<int> &&selection)
{
	this->selection = selection;
}

DWORD Model::GetSetMask()
{
	DWORD result = 0xFFFFFFFF;

	for (auto i : this->selection)
		result &= this->files[i].GetUpdated();

	return result;
}

DWORD Model::GetUnsetMask()
{
	DWORD result = 0xFFFFFFFF;

	for (auto i : this->selection)
		result &= ~this->files[i].GetUpdated();

	return result;
}

bool Model::IsSelectionEmpty()
{
	return this->selection.empty();
}

size_t Model::GetSelectionSize()
{
	return this->selection.size();
}
