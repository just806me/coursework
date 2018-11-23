#pragma once

#include <windows.h>
#include <string>

class File
{
public:
	File(std::string &);
	bool Save();
	void Restore();
	void Set(DWORD, int);
	void Set(DWORD);
	DWORD GetOriginal();
	DWORD GetUpdated();

	static const constexpr std::pair<const DWORD, const char *> ATTRIBUTES[] = {
		{ FILE_ATTRIBUTE_ARCHIVE,               "�����������"           },
		{ FILE_ATTRIBUTE_HIDDEN,                "����������"            },
		{ FILE_ATTRIBUTE_NOT_CONTENT_INDEXED,   "�� ������������"       },
		{ FILE_ATTRIBUTE_READONLY,              "���� ��� �������"      },
		{ FILE_ATTRIBUTE_OFFLINE,               "������"                },
		{ FILE_ATTRIBUTE_SYSTEM,                "���������"             },
		{ FILE_ATTRIBUTE_TEMPORARY,             "����������"            },
		{ FILE_ATTRIBUTE_NORMAL,                "����������"            },
		{ FILE_ATTRIBUTE_COMPRESSED,            "���������"             },
		{ FILE_ATTRIBUTE_ENCRYPTED,             "������������"          },
		{ FILE_ATTRIBUTE_SPARSE_FILE,           "����������"           },
		{ FILE_ATTRIBUTE_NO_SCRUB_DATA,         "NO_SCRUB_DATA"         },
		{ FILE_ATTRIBUTE_INTEGRITY_STREAM,      "INTEGRITY_STREAM"      },
		{ FILE_ATTRIBUTE_RECALL_ON_DATA_ACCESS, "RECALL_ON_DATA_ACCESS" },
		{ FILE_ATTRIBUTE_RECALL_ON_OPEN,        "RECALL_ON_OPEN"        },
		{ FILE_ATTRIBUTE_REPARSE_POINT,         "REPARSE_POINT"         },
	};
	static const constexpr size_t ATTRIBUTES_COUNT = _countof(File::ATTRIBUTES);

private:
	std::string path;
	DWORD original, updated;
};
