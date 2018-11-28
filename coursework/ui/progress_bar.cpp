#include "progress_bar.h"

UI::ProgressBar::ProgressBar(HINSTANCE instance, HFONT font, HWND parent, int x, int y, int width, int height)
{
	this->handle = CreateWindowEx(NULL, PROGRESS_CLASS, NULL, WS_VISIBLE | WS_CHILD | PBS_SMOOTH,
		x, y, width, height, parent, NULL, instance, NULL);

	if (!this->handle)
		throw new Exception("CreateWindow failed");

	this->InitializeElement(font);
	SendMessage(this->handle, PBM_SETSTEP, 1, NULL);
}

void UI::ProgressBar::SetRange(size_t range)
{
	SendMessage(this->handle, PBM_SETRANGE32, NULL, range);
	SendMessage(this->handle, PBM_SETPOS, NULL, NULL);
}

void UI::ProgressBar::Step()
{
	SendMessage(this->handle, PBM_STEPIT, NULL, NULL);
}
