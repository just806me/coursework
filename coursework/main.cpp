#pragma comment(lib, "Comctl32.lib")

#include "exception.h"
#include "view.h"

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int)
{
	try
	{
		View(instance).Run();
	}
	catch (Exception ex)
	{
		MessageBox(NULL, ex.what(), "Помилка", MB_OK | MB_ICONERROR);
	}

	return 0;
}
