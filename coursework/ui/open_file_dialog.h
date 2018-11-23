#pragma once

#include <windows.h>
#include <vector>
#include <string>

namespace UI
{
	class OpenFileDialog
	{
	public:
		OpenFileDialog(HINSTANCE, HWND, const char *);
		~OpenFileDialog();
		bool Show();
		std::vector<std::string> GetSelectedFiles();

	private:
		char *buffer;
		OPENFILENAME ofn;

		static const constexpr size_t BUFFER_SIZE = 16 * 1024 * 1024;
	};
}
