#include "file.h"

File::File(std::string &path) : path(path)
{
	this->updated = this->original = GetFileAttributes(this->path.c_str());
}

bool File::Save()
{
	if (SetFileAttributes(this->path.c_str(), this->updated))
	{
		this->original = this->updated = GetFileAttributes(this->path.c_str());
		return true;
	}

	return false;
}

void File::Restore()
{
	this->updated = this->original;
}

void File::Set(DWORD attribute, int value)
{
	switch (value)
	{
	case BST_CHECKED:
		this->updated |= attribute;
		break;

	case BST_UNCHECKED:
		this->updated &= ~attribute;
		break;

	case BST_INDETERMINATE:
		if (this->original & attribute)
			this->updated |= attribute;
		else
			this->updated &= ~attribute;
		break;
	}
}

void File::Set(DWORD value)
{
	this->updated = value;
}

DWORD File::GetOriginal()
{
	return this->original;
}

DWORD File::GetUpdated()
{
	return this->updated;
}
